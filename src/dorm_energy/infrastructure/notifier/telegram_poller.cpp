#include "dorm_energy/infrastructure/notifier/telegram_poller.hpp"

#include <chrono>

namespace dorm_energy::notifier
{
    TelegramPoller::TelegramPoller(const TelegramConfig &config)
        : config_(config), baseUrl_("https://api.telegram.org/bot" + config_.botToken)
    {
    }

    TelegramPoller::~TelegramPoller() { stop(); }

    void TelegramPoller::start()
    {
        if (running_)
        {
            return;
        }

        running_ = true;
        pollThread_ = std::thread(&TelegramPoller::pollingLoop, this);
    }

    void TelegramPoller::stop()
    {
        if (!running_)
        {
            return;
        }

        running_ = false;

        if (pollThread_.joinable())
        {
            pollThread_.join();
        }
    }

    void TelegramPoller::pollingLoop()
    {
        while (running_)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    bool TelegramPoller::handleCallbackQuery(const std::string &, long, long) { return false; }

    bool TelegramPoller::editMessage(long, long, const std::string &) { return false; }
} // namespace dorm_energy::notifier
