// include/dorm_energy/logging/logger.hpp
#pragma once

#include <string>
#include <format>
#include <chrono>
#include <iostream>

namespace dorm_energy::logging
{
    // можно попробовать библиотеку логер
    class Logger
    {
    public:
        explicit Logger(std::string name = "dorm-energy");

        void info(const std::string &message) const;
        void debug(const std::string &message) const;
        void warn(const std::string &message) const;
        void error(const std::string &message) const;

    private:
        void log(const std::string &level, const std::string &message) const;
        std::string name_;
    };

} // namespace dorm_energy::logging