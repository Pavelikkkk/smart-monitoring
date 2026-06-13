#include "dorm_energy/infrastructure/notifier/telegram_queue.hpp"
#include <iostream>

namespace dorm_energy::notifier
{

    void TelegramQueue::push(const core::SensorReading &reading,
                             core::AlertSeverity severity,
                             const std::string &reason)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (queue_.size() >= MAX_QUEUE_SIZE)
        {
            std::cout << "[TelegramQueue] Queue is full (" << MAX_QUEUE_SIZE << "). Removing oldest message.\n";
            queue_.erase(queue_.begin());
        }

        queue_.push_back(QueuedAlert{reading, severity, reason, std::chrono::system_clock::now()});
    }

    std::vector<QueuedAlert> TelegramQueue::getAllAndClear()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<QueuedAlert> result = std::move(queue_);
        queue_.clear();
        return result;
    }

    bool TelegramQueue::empty() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    std::size_t TelegramQueue::size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

} // namespace dorm_energy::notifier