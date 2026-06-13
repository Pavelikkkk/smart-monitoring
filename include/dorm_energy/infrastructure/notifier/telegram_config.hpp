#pragma once
#include "dorm_energy/application/config/app_config.hpp"
#include <string>

namespace dorm_energy::notifier
{
    struct TelegramConfig
    {
        std::string botToken;
        std::string chatId;
        bool enabled = false;

        static TelegramConfig fromAppConfig(const application::AppConfig &config);
    };
} // namespace dorm_energy::notifier