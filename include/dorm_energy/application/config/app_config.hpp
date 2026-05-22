// include/dorm_energy/application/config/app_config.hpp
#pragma once

#include "dorm_energy/application/cli/command_options.hpp"

#include <string>

namespace dorm_energy::application
{
    struct AppConfig
    {
        // Общие
        bool verbose{false};
        std::string logLevel{"info"};

        // Симуляция
        int simulationDays{30};
        unsigned randomSeed{42};
        bool injectAnomalies{false};
        double anomalyRate{0.03};

        // База данных
        std::string dbHost{"localhost"};
        std::string dbPort{"5432"};
        std::string dbName{"dorm_energy"};
        std::string dbUser{"postgres"};
        std::string dbPassword{};

        // MQTT
        std::string mqttBroker{"tcp://127.0.0.1:1883"};
        std::string mqttClientId{"dorm-energy"};
        std::string mqttTopic{"devices/+/power"};

        // Уведомления
        std::string notifierType{"console"};

        static AppConfig loadFromEnvironment();

        /**
         * @brief Применяет параметры из командной строки (перезапись)
         */
        void overrideFromCli(const cli::CommandOptions &cli);

        std::string getDbConnectionString() const;
        std::string validate() const;
        void clearSensitiveData();
    };

} // namespace dorm_energy::application