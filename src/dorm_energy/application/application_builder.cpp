// src/dorm_energy/application/application_builder.cpp
#include "dorm_energy/application/application_builder.hpp"
#include "dorm_energy/application/application.hpp"

#include "dorm_energy/infrastructure/logging/spdlog_logger.hpp"
#include "dorm_energy/infrastructure/simulation/synthetic_data_generator.hpp"
#include "dorm_energy/infrastructure/detection/rule_based_detector.hpp"
#include "dorm_energy/infrastructure/storage/postgres_repository.hpp"
#include "dorm_energy/infrastructure/handlers/message_handler.hpp"
#include "dorm_energy/infrastructure/notifier/console_notifier.hpp"
#include "dorm_energy/infrastructure/mqtt/mqtt_client.hpp"
#include "dorm_energy/infrastructure/cli/cli_parser.hpp"
#include "dorm_energy/infrastructure/detection/onnx_detector.hpp"
#include "dorm_energy/infrastructure/detection/hybrid_detector.hpp"
#include "dorm_energy/infrastructure/web/server/web_server.hpp"
#include "dorm_energy/application/auth/auth_service.hpp"
#include "dorm_energy/infrastructure/auth/jwt_service.hpp"
#include "dorm_energy/infrastructure/auth/openssl_password_hasher.hpp"

namespace dorm_energy::application
{

    ApplicationBuilder::ApplicationBuilder() = default;

    ApplicationBuilder &ApplicationBuilder::withConfig(AppConfig config)
    {
        config_ = std::move(config);
        return *this;
    }

    std::shared_ptr<logging::ILogger> ApplicationBuilder::createLogger()
    {
        static std::shared_ptr<logging::ILogger> logger;

        if (!logger)
        {
            logger = std::make_shared<logging::SpdlogLogger>(config_);
        }

        return logger;
    }

    std::unique_ptr<simulation::IDataGenerator>
    ApplicationBuilder::createGenerator()
    {
        auto repository =
            createRepository();

        return std::make_unique<
            simulation::SyntheticDataGenerator>(
            config_.getRandomSeed(),
            config_.getInjectAnomalies(),
            config_.getAnomalyRate(),
            repository);
    }

    std::shared_ptr<AuthService>
    ApplicationBuilder::createAuthService()
    {
        auto repository =
            createRepository();

        auto passwordHasher =
            std::make_shared<
                OpenSslPasswordHasher>();

        auto jwtService =
            std::make_shared<
                JwtService>(
                "super-secret-key");

        return std::make_shared<
            AuthService>(
            repository,
            passwordHasher,
            jwtService);
    }

    std::unique_ptr<detection::IStateDetector> ApplicationBuilder::createDetector()
    {
        auto ruleDetector =
            std::make_unique<
                detection::RuleBasedDetector>(
                25.0);

        auto mlDetector =
            std::make_unique<
                detection::OnnxDetector>(
                "../../ml/models/anomaly_autoencoder.onnx");

        return std::make_unique<
            detection::HybridDetector>(
            std::move(ruleDetector),
            std::move(mlDetector));
    }

    std::shared_ptr<storage::IMeasurementRepository> ApplicationBuilder::createRepository()
    {
        if (repository_)
            return repository_;

        try
        {
            std::string connStr = config_.getDbConnectionString();

            std::cout << "[Builder] Connecting to TimescaleDB: "
                      << config_.getDbHost() << ":" << config_.getDbPort()
                      << "/" << config_.getDbName() << std::endl;

            repository_ = std::make_shared<storage::PostgresMeasurementRepository>(
                connStr,
                config_.getDbMaxBufferSize());

            std::cout << "[Builder] PostgresMeasurementRepository created successfully (buffer = "
                      << config_.getDbMaxBufferSize() << ")\n";
            return repository_;
        }
        catch (const std::exception &e)
        {
            std::cerr << "[Builder] Failed to create repository: " << e.what() << std::endl;
            throw;
        }
    }

    std::unique_ptr<application::INotifier> ApplicationBuilder::createNotifier()
    {
        auto service = std::make_unique<application::NotifierService>();

        service->addNotifier(std::make_unique<notifier::ConsoleNotifier>());

        if (config_.isTelegramEnabled() &&
            !config_.getTelegramBotToken().empty() &&
            !config_.getTelegramChatId().empty())
        {
            try
            {
                auto telegramNotifier = std::make_unique<notifier::TelegramNotifier>(config_);
                service->addNotifier(std::move(telegramNotifier));

                std::cout << "[Builder] ✅ TelegramNotifier successfully enabled\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "[Builder] ❌ Failed to initialize TelegramNotifier: " << e.what() << std::endl;
            }
        }
        else if (config_.isTelegramEnabled())
        {
            std::cout << "[Builder] ⚠️ Telegram enabled in config, but token or chat_id is missing\n";
        }

        return service;
    }
    std::shared_ptr<dorm_energy::detection::RoomStateAggregator> ApplicationBuilder::createAggregator()
    {
        if (!aggregator_)
        {
            aggregator_ =
                std::make_shared<
                    dorm_energy::detection::RoomStateAggregator>();
        }

        return aggregator_;
    }
    
    std::unique_ptr<application::IMessageHandler> ApplicationBuilder::createMessageHandler()
    {
        return std::make_unique<handlers::MessageHandler>(
            createDetector(),
            createRepository(),
            createNotifier(),
            createAggregator());
    }

    std::shared_ptr<mqtt::MqttClient> ApplicationBuilder::createSharedMqttClient()
    {
        static std::shared_ptr<mqtt::MqttClient> sharedClient;

        if (!sharedClient)
        {
            sharedClient = std::make_shared<mqtt::MqttClient>();

            if (!config_.getMqttBroker().empty())
            {
                sharedClient->setMode(mqtt::MqttMode::Real);
            }
            else
            {
                sharedClient->setMode(mqtt::MqttMode::Simulation);
            }
        }

        return sharedClient;
    }

    std::shared_ptr<mqtt::IMqttConnection> ApplicationBuilder::createMqttConnection()
    {
        return createSharedMqttClient();
    }

    std::shared_ptr<mqtt::IMqttSubscription> ApplicationBuilder::createMqttSubscription()
    {
        return createSharedMqttClient();
    }

    std::shared_ptr<mqtt::IMqttMessageDispatcher> ApplicationBuilder::createMqttDispatcher()
    {
        return createSharedMqttClient();
    }

    std::unique_ptr<cli::CliParser> ApplicationBuilder::createCliParser()
    {
        return std::make_unique<cli::CliParser>();
    }

    std::unique_ptr<SimulateCommand> ApplicationBuilder::createSimulateCommand()
    {
        return std::make_unique<SimulateCommand>(
            createLogger(),
            createGenerator(),
            createDetector(),
            createRepository());
    }
    std::shared_ptr<web::WebServer> ApplicationBuilder::createWebServer()
    {
        return std::make_shared<web::WebServer>(
            createAggregator(),
            createRepository(),
            createAuthService());
    }

    std::unique_ptr<DaemonCommand> ApplicationBuilder::createDaemonCommand()
    {
        return std::make_unique<DaemonCommand>(
            createLogger(),
            config_,
            createMqttConnection(),
            createMqttSubscription(),
            createMqttDispatcher(),
            createMessageHandler(),
            createWebServer());
    }

    std::unique_ptr<Application> ApplicationBuilder::build()
    {
        config_ = AppConfig::load();

        repository_ = createRepository();

        auto cliParser = createCliParser();
        auto simulateCmd = createSimulateCommand();
        auto daemonCmd = createDaemonCommand();

        return std::make_unique<Application>(
            config_,
            std::move(cliParser),
            std::move(simulateCmd),
            std::move(daemonCmd),
            repository_);
    }

} // namespace dorm_energy::application
