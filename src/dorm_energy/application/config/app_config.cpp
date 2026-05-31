// src/dorm_energy/application/config/app_config.cpp
#include "dorm_energy/application/config/app_config.hpp"
#include "dorm_energy/application/cli/command_options.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <filesystem>

namespace dorm_energy::application
{

    AppConfig AppConfig::load()
    {
        AppConfig config = loadFromEnvFile(".env");
        AppConfig envConfig = loadFromEnvironment();

        if (!envConfig.getDbHost().empty())
            config.dbHost_ = envConfig.getDbHost();
        if (!envConfig.getDbPort().empty())
            config.dbPort_ = envConfig.getDbPort();
        if (!envConfig.getDbName().empty())
            config.dbName_ = envConfig.getDbName();
        if (!envConfig.getDbUser().empty())
            config.dbUser_ = envConfig.getDbUser();
        if (!envConfig.getDbPassword().empty())
            config.dbPassword_ = envConfig.getDbPassword();

        return config;
    }

    AppConfig AppConfig::loadFromEnvFile(const std::string &filename)
    {
        AppConfig config;

        std::filesystem::path exePath;
        try
        {
            exePath = std::filesystem::current_path() / filename;

            if (!std::filesystem::exists(exePath))
            {
                auto parentPath = std::filesystem::current_path().parent_path();
                if (parentPath.filename() == "build" ||
                    parentPath.filename() == "Debug" ||
                    parentPath.filename() == "Release")
                {
                    exePath = parentPath.parent_path() / filename;
                }
            }
        }
        catch (...)
        {
            exePath = filename;
        }

        std::ifstream file(exePath);
        if (!file.is_open())
        {
            std::cout << "[Config] .env file not found\n";
            return config;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                continue;

            auto pos = line.find('=');
            if (pos == std::string::npos)
                continue;

            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            if (key == "DB_HOST")
                config.dbHost_ = value;
            else if (key == "DB_PORT")
                config.dbPort_ = value;
            else if (key == "DB_NAME")
                config.dbName_ = value;
            else if (key == "DB_USER")
                config.dbUser_ = value;
            else if (key == "DB_PASSWORD")
                config.dbPassword_ = value;
            else if (key == "MQTT_BROKER")
                config.mqttBroker_ = value;
            else if (key == "MQTT_TOPIC")
                config.mqttTopic_ = value;
            else if (key == "LOG_LEVEL")
                config.logLevel_ = value;
            else if (key == "SIMULATION_DAYS")
            {
                try
                {
                    config.simulationDays_ = std::stoi(value);
                }
                catch (...)
                {
                }
            }
            else if (key == "DB_MAX_BUFFER_SIZE")
            {
                try
                {
                    config.dbMaxBufferSize_ = std::stoul(value);
                }
                catch (...)
                {
                }
            }
            else if (key == "TELEGRAM_ENABLED")
            {
                std::string v = value;
                std::transform(v.begin(), v.end(), v.begin(), ::tolower);
                config.telegramEnabled_ = (v == "true" || v == "1" || v == "on" || v == "yes");
            }
            else if (key == "TELEGRAM_BOT_TOKEN")
                config.telegramBotToken_ = value;
            else if (key == "TELEGRAM_CHAT_ID")
                config.telegramChatId_ = value;
        }
        return config;
    }

    AppConfig AppConfig::loadFromEnvironment()
    {
        AppConfig config;

        if (const char *val = std::getenv("MQTT_BROKER"))
            config.mqttBroker_ = val;
        if (const char *val = std::getenv("MQTT_TOPIC"))
            config.mqttTopic_ = val;
        if (const char *val = std::getenv("DB_HOST"))
            config.dbHost_ = val;
        if (const char *val = std::getenv("DB_PORT"))
            config.dbPort_ = val;
        if (const char *val = std::getenv("DB_NAME"))
            config.dbName_ = val;
        if (const char *val = std::getenv("DB_USER"))
            config.dbUser_ = val;
        if (const char *val = std::getenv("DB_PASSWORD"))
            config.dbPassword_ = val;
        if (const char *val = std::getenv("LOG_LEVEL"))
            config.logLevel_ = val;
        if (const char *val = std::getenv("TELEGRAM_ENABLED"))
        {
            std::string s = val;
            config.telegramEnabled_ = (s == "1" || s == "true" || s == "TRUE" || s == "on");
        }

        if (const char *val = std::getenv("TELEGRAM_BOT_TOKEN"))
            config.telegramBotToken_ = val;

        if (const char *val = std::getenv("TELEGRAM_CHAT_ID"))
            config.telegramChatId_ = val;

        if (const char *val = std::getenv("SIMULATION_DAYS"))
        {
            try
            {
                config.simulationDays_ = std::stoi(val);
            }
            catch (...)
            {
            }
        }
        if (const char *val = std::getenv("DB_MAX_BUFFER_SIZE"))
        {
            try
            {
                config.dbMaxBufferSize_ = std::stoul(val);
            }
            catch (...)
            {
            }
        }

        return config;
    }

    void AppConfig::overrideFromCli(const cli::CommandOptions &cli)
    {
        if (cli.common.verbose)
            verbose_ = true;

        if (cli.isSimulate())
        {
            simulationDays_ = cli.simulateDays;
            injectAnomalies_ = cli.injectAnomalies;
            anomalyRate_ = cli.anomalyRate;
        }

        if (!cli.mqttBroker.empty())
            mqttBroker_ = cli.mqttBroker;
        if (!cli.mqttTopic.empty())
            mqttTopic_ = cli.mqttTopic;
    }

    std::string AppConfig::getDbConnectionString() const
    {
        std::ostringstream oss;
        oss << "host=" << dbHost_
            << " port=" << dbPort_
            << " dbname=" << dbName_
            << " user=" << dbUser_;

        if (!dbPassword_.empty())
            oss << " password=" << dbPassword_;

        return oss.str();
    }

    std::string AppConfig::validate() const
    {
        if (mqttBroker_.empty())
            return "MQTT broker address is not set";
        if (dbHost_.empty() || dbName_.empty())
            return "Database configuration is incomplete";
        if (simulationDays_ <= 0)
            return "Simulation days must be positive";
        return "";
    }

    void AppConfig::clearSensitiveData()
    {
        dbPassword_.clear();
        dbPassword_.shrink_to_fit();
    }

} // namespace dorm_energy::application