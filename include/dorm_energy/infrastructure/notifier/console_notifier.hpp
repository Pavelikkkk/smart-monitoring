// include/dorm_energy/infrastructure/notifier/console_notifier.hpp
#pragma once

#include "dorm_energy/application/inotifier.hpp"
#include "dorm_energy/core/alert_severity.hpp"
#include "dorm_energy/core/measurement.hpp"

namespace dorm_energy::notifier
{
    class ConsoleNotifier : public application::INotifier
    {
    public:
        ConsoleNotifier() = default;

        bool sendAlert(const core::RoomState &state, const detection::AnomalyInfo &info) override;

        std::size_t sendAlerts(const std::vector<core::RoomState> &states,
                               const detection::AnomalyInfo &info) override;

    private:
        std::string formatAlert(const core::RoomState &state,
                                const detection::AnomalyInfo &info) const;
    };
} // namespace dorm_energy::notifier