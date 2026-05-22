// include/dorm_energy/application/cli/command_options.hpp
#pragma once

#include "command_type.hpp"
#include <string>

namespace dorm_energy::cli
{
    struct CommonOptions
    {
        bool verbose{false};
        bool help{false};
    };

    /**
     * @brief Параметры командной строки
     */
    struct CommandOptions // тянет за собой все параметры подумать о разбиении на несколько структур
    {
        CommandType type{CommandType::Unknown};

        CommonOptions common;

        int simulateDays{30}; 
        bool injectAnomalies{false};
        double anomalyRate{0.03};

        std::string mqttBroker; 
        std::string mqttTopic;

        bool isSimulate() const noexcept { return type == CommandType::Simulate; }
        bool isDaemon() const noexcept { return type == CommandType::Daemon; }
        bool isHelp() const noexcept { return type == CommandType::Help; }
    };

} // namespace dorm_energy::cli