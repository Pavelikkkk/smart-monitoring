// include/dorm_energy/domain/logging/ilogger.hpp
#pragma once

#include <string>

namespace dorm_energy::logging
{
    /**
     * @brief Уровни логирования

     */
    enum class LogLevel
    {
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };
    /**
     * @brief Интерфейс для логирования в проекте
     */
    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        /**
         * @brief Основной метод логирования
         */
        virtual void log(LogLevel level, const std::string &message) = 0;

        void debug(const std::string &msg) { log(LogLevel::Debug, msg); }
        void info(const std::string &msg) { log(LogLevel::Info, msg); }
        void warn(const std::string &msg) { log(LogLevel::Warn, msg); }
        void error(const std::string &msg) { log(LogLevel::Error, msg); }
        void critical(const std::string &msg) { log(LogLevel::Critical, msg); }
    };

} // namespace dorm_energy::domain::logging