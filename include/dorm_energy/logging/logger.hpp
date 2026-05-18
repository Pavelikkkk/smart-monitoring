// include/dorm_energy/logging/logger.hpp
#pragma once

#include <string>

#include "../interfaces.hpp"

namespace dorm_energy::logging
{
    // можно попробовать библиотеку логер
    class Logger : public ILogger
    {
    public:
        explicit Logger(std::string name = "dorm-energy");

        void info(const std::string &message) const override;
        void debug(const std::string &message) const override;
        void warn(const std::string &message) const override;
        void error(const std::string &message) const override;

    private: 
        void log(const std::string &level, const std::string &message) const;
        std::string name_;
    };

} // namespace dorm_energy::logging