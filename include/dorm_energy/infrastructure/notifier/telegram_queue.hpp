#pragma once

#include "dorm_energy/core/alert_severity.hpp"
#include "dorm_energy/core/measurement.hpp"
#include <chrono>
#include <mutex>
#include <string>
#include <vector>

namespace dorm_energy::notifier
{
    struct QueuedAlert
    {
        core::SensorReading reading;
        core::AlertSeverity severity;
        std::string reason;
        std::chrono::system_clock::time_point queuedAt;
    };

    class TelegramQueue
    {
    public:
        static constexpr std::size_t MAX_QUEUE_SIZE = 500;

        void push(const core::SensorReading &reading, core::AlertSeverity severity,
                  const std::string &reason);

        std::vector<QueuedAlert> getAllAndClear();
        bool empty() const;
        std::size_t size() const;

    private:
        std::vector<QueuedAlert> queue_;
        mutable std::mutex mutex_;
    };
} // namespace dorm_energy::notifier