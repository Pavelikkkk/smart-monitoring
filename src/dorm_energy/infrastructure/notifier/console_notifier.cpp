#include "dorm_energy/infrastructure/notifier/console_notifier.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

namespace dorm_energy::notifier
{

    std::string ConsoleNotifier::formatAlert(
        const core::RoomState &state,
        const detection::AnomalyInfo &info) const
    {
        std::ostringstream oss;

        oss << "[ANOMALY] "
            << state.roomId
            << " -> "
            << info.anomalyType;

        if (info.score > 0.0)
        {
            oss << " score="
                << std::fixed
                << std::setprecision(3)
                << info.score;
        }

        return oss.str();
    }

    bool ConsoleNotifier::sendAlert(
        const core::RoomState &state,
        const detection::AnomalyInfo &info)
    {
        std::cout
            << "["
            << core::toString(info.severity)
            << "] "
            << formatAlert(state, info)
            << '\n';

        return true;
    }

    std::size_t ConsoleNotifier::sendAlerts(
        const std::vector<core::RoomState> &states,
        const detection::AnomalyInfo &info)
    {
        if (states.empty())
        {
            return 0;
        }

        std::size_t sent = 0;

        for (const auto &state : states)
        {
            if (sendAlert(state, info))
            {
                ++sent;
            }
        }

        std::cout
            << "Dispatched "
            << sent
            << " alerts to console\n";

        return sent;
    }

} // namespace dorm_energy::notifier