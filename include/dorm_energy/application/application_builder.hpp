// include/dorm_energy/application/application_builder.hpp
#pragma once

#include "dorm_energy/application/application.hpp"
#include "dorm_energy/application/config/app_config.hpp"
#include "dorm_energy/application/commands/simulate_command.hpp"
#include "dorm_energy/application/commands/daemon_command.hpp"
#include "dorm_energy/infrastructure/cli/cli_parser.hpp"

#include "dorm_energy/domain/logging/ilogger.hpp"
#include "dorm_energy/domain/simulation/idata_generator.hpp"
#include "dorm_energy/domain/detection/ianomaly_detector.hpp"
#include "dorm_energy/domain/storage/imeasurement_repository.hpp"
#include "dorm_energy/application/imessage_handler.hpp"
#include "dorm_energy/application/inotifier.hpp"
#include "dorm_energy/domain/mqtt/imqtt_connection.hpp"
#include "dorm_energy/domain/mqtt/imqtt_subscription.hpp"
#include "dorm_energy/domain/mqtt/imqtt_message_dispatcher.hpp"

#include <memory>

namespace dorm_energy::application
{

    /**
     * @brief Отвечает за создание всех зависимостей и сборку Application.
     */
    class ApplicationBuilder
    {
    public:
        ApplicationBuilder();

        /**
         * @brief Устанавливает конфигурацию для билдера
         */
        ApplicationBuilder &withConfig(AppConfig config);

        std::unique_ptr<Application> build();

    private:
        AppConfig config_;

        std::unique_ptr<dorm_energy::logging::ILogger> createLogger();
        std::unique_ptr<dorm_energy::simulation::IDataGenerator> createGenerator();
        std::unique_ptr<dorm_energy::detection::IAnomalyDetector> createDetector();
        std::unique_ptr<dorm_energy::storage::IMeasurementRepository> createRepository();
        std::unique_ptr<dorm_energy::application::IMessageHandler> createMessageHandler();
        std::unique_ptr<dorm_energy::application::INotifier> createNotifier();

        std::unique_ptr<dorm_energy::mqtt::IMqttConnection> createMqttConnection();
        std::unique_ptr<dorm_energy::mqtt::IMqttSubscription> createMqttSubscription();
        std::unique_ptr<dorm_energy::mqtt::IMqttMessageDispatcher> createMqttDispatcher();

        std::unique_ptr<dorm_energy::cli::CliParser> createCliParser();

        std::unique_ptr<dorm_energy::application::SimulateCommand> createSimulateCommand();
        std::unique_ptr<dorm_energy::application::DaemonCommand> createDaemonCommand();

        void applyCliOverrides(dorm_energy::cli::CommandOptions cliOptions);
    };

} // namespace dorm_energy::application