// src/infrastructure/logging/spdlog_logger.cpp
#include "dorm_energy/infrastructure/logging/spdlog_logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace dorm_energy::infrastructure::logging
{

    SpdlogLogger::SpdlogLogger(const std::string &name)
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        logger_ = std::make_shared<spdlog::logger>(name, console_sink);

        logger_->set_level(spdlog::level::debug);       
        logger_->set_pattern("[%H:%M:%S] [%^%l%$] %v"); 
    }

    void SpdlogLogger::log(domain::logging::LogLevel level, const std::string &message)
    {
        switch (level)
        {
        case domain::logging::LogLevel::Debug:
            logger_->debug(message);
            break;
        case domain::logging::LogLevel::Info:
            logger_->info(message);
            break;
        case domain::logging::LogLevel::Warn:
            logger_->warn(message);
            break;
        case domain::logging::LogLevel::Error:
            logger_->error(message);
            break;
        case domain::logging::LogLevel::Critical:
            logger_->critical(message);
            break;
        }
    }

} // namespace dorm_energy::infrastructure::logging