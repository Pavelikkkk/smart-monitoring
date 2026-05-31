// include/dorm_energy/application/config/app_config.hpp
#pragma once

#include "dorm_energy/application/cli/command_options.hpp"
#include <string>
#include <cstddef>

namespace dorm_energy::application
{
    class AppConfig
    {
    public:
        bool isVerbose() const { return verbose_; }
        const std::string &getLogLevel() const { return logLevel_; }

        int getSimulationDays() const { return simulationDays_; }
        unsigned getRandomSeed() const { return randomSeed_; }
        bool getInjectAnomalies() const { return injectAnomalies_; }
        double getAnomalyRate() const { return anomalyRate_; }

        const std::string &getDbHost() const { return dbHost_; }
        const std::string &getDbPort() const { return dbPort_; }
        const std::string &getDbName() const { return dbName_; }
        const std::string &getDbUser() const { return dbUser_; }
        const std::string &getDbPassword() const { return dbPassword_; }
        std::size_t getDbMaxBufferSize() const { return dbMaxBufferSize_; }

        const std::string &getMqttBroker() const { return mqttBroker_; }
        const std::string &getMqttClientId() const { return mqttClientId_; }
        const std::string &getMqttTopic() const { return mqttTopic_; }

        const std::string &getTelegramBotToken() const { return telegramBotToken_; }
        const std::string &getTelegramChatId() const { return telegramChatId_; }
        bool isTelegramEnabled() const { return telegramEnabled_; }

        static AppConfig load();
        static AppConfig loadFromEnvironment();
        static AppConfig loadFromEnvFile(const std::string &filename = ".env");

        void overrideFromCli(const cli::CommandOptions &cli);
        std::string getDbConnectionString() const;
        std::string validate() const;
        void clearSensitiveData();

    private:
        bool verbose_{false};
        std::string logLevel_{"info"};

        int simulationDays_{30};
        unsigned randomSeed_{42};
        bool injectAnomalies_{true};
        double anomalyRate_{0.03};

        std::string dbHost_{"localhost"};
        std::string dbPort_{"5432"};
        std::string dbName_{"dorm_energy"};
        std::string dbUser_{"postgres"};
        std::string dbPassword_{};
        std::size_t dbMaxBufferSize_{2000};

        std::string mqttBroker_{"tcp://127.0.0.1:1883"};
        std::string mqttClientId_{"dorm-energy"};
        std::string mqttTopic_{"devices/+/power"};

        std::string notifierType_{"console"};
        bool telegramEnabled_{false};
        std::string telegramBotToken_;
        std::string telegramChatId_;
    };

} // namespace dorm_energy::application