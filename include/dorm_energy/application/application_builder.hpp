// include/dorm_energy/application/application_builder.hpp
#pragma once

#include "dorm_energy/application/application.hpp"
#include "dorm_energy/application/config/app_config.hpp"
#include "dorm_energy/application/commands/simulate_command.hpp"
#include "dorm_energy/application/commands/daemon_command.hpp"
#include "dorm_energy/application/imessage_handler.hpp"
#include "dorm_energy/application/inotifier.hpp"

#include "dorm_energy/domain/logging/ilogger.hpp"
#include "dorm_energy/domain/simulation/idata_generator.hpp"
#include "dorm_energy/domain/detection/istate_detector.hpp"
#include "dorm_energy/domain/storage/imeasurement_repository.hpp"
#include "dorm_energy/domain/mqtt/imqtt_connection.hpp"
#include "dorm_energy/domain/mqtt/imqtt_subscription.hpp"
#include "dorm_energy/domain/mqtt/imqtt_message_dispatcher.hpp"
#include "dorm_energy/infrastructure/mqtt/mqtt_client.hpp"

#include "dorm_energy/infrastructure/cli/cli_parser.hpp"
#include "dorm_energy/application/notifier_service.hpp"
#include "dorm_energy/infrastructure/notifier/console_notifier.hpp"
#include "dorm_energy/infrastructure/notifier/telegram_notifier.hpp"
#include "dorm_energy/infrastructure/web/server/web_server.hpp"
#include "dorm_energy/application/auth/auth_service.hpp"

#include <memory>

namespace dorm_energy::application
{

    /**
     * @brief
     */
    class ApplicationBuilder
    {
    public:
        ApplicationBuilder();

        /**
         * @brief
         */
        ApplicationBuilder &withConfig(AppConfig config);

        std::unique_ptr<Application> build();

    private:
        AppConfig config_;
        std::shared_ptr<storage::IMeasurementRepository> repository_;
        std::shared_ptr<dorm_energy::detection::RoomStateAggregator> aggregator_;

        std::shared_ptr<dorm_energy::logging::ILogger> createLogger();

        std::unique_ptr<dorm_energy::simulation::IDataGenerator> createGenerator();
        std::unique_ptr<dorm_energy::detection::IStateDetector> createDetector();
        std::shared_ptr<dorm_energy::storage::IMeasurementRepository> createRepository();
        std::unique_ptr<dorm_energy::application::IMessageHandler> createMessageHandler();
        std::unique_ptr<dorm_energy::application::INotifier> createNotifier();

        std::shared_ptr<dorm_energy::mqtt::MqttClient> createSharedMqttClient();

        std::shared_ptr<dorm_energy::mqtt::IMqttConnection> createMqttConnection();
        std::shared_ptr<dorm_energy::mqtt::IMqttSubscription> createMqttSubscription();
        std::shared_ptr<dorm_energy::mqtt::IMqttMessageDispatcher> createMqttDispatcher();

        std::unique_ptr<dorm_energy::cli::CliParser> createCliParser();

        std::unique_ptr<dorm_energy::application::SimulateCommand> createSimulateCommand();
        std::unique_ptr<dorm_energy::application::DaemonCommand> createDaemonCommand();
        std::shared_ptr<dorm_energy::web::WebServer> createWebServer();
        std::shared_ptr<dorm_energy::detection::RoomStateAggregator> createAggregator();
        std::shared_ptr<AuthService> createAuthService();

        void applyCliOverrides(dorm_energy::cli::CommandOptions cliOptions);
    };

} // namespace dorm_energy::application