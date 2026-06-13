// include/dorm_energy/application/inotifier.hpp
#pragma once

#include "dorm_energy/core/alert_severity.hpp"
#include "dorm_energy/core/measurement.hpp"
#include "dorm_energy/core/room_state.hpp"
#include "dorm_energy/domain/detection/anomaly_info.hpp"
#include <string>
#include <vector>

namespace dorm_energy::application
{
    class INotifier
    {
    public:
        virtual ~INotifier() = default;

        virtual bool sendAlert(const core::RoomState &state,
                               const detection::AnomalyInfo &info) = 0;

        virtual std::size_t sendAlerts(const std::vector<core::RoomState> &states,
                                       const detection::AnomalyInfo &info) = 0;
    };
} // namespace dorm_energy::application