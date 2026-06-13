#pragma once

#include "telegram_config.hpp"
#include <atomic>
#include <string>
#include <thread>

namespace dorm_energy::notifier
{
    class TelegramPoller
    {
    public:
        explicit TelegramPoller(const TelegramConfig &config);
        ~TelegramPoller();

        void start();
        void stop();

    private:
        TelegramConfig config_;
        std::string baseUrl_;

        std::thread pollThread_;

        std::atomic<bool> running_{false};
        std::atomic<long> lastUpdateId_{0};

        void pollingLoop();

        bool handleCallbackQuery(const std::string &callbackData, long messageId, long chatId);

        bool editMessage(long chatId, long messageId, const std::string &newText);
    };
} // namespace dorm_energy::notifier