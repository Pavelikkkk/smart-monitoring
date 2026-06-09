#pragma once

#include "dorm_energy/core/room_state.hpp"
#include "dorm_energy/core/measurement.hpp"

#include <deque>
#include <optional>
#include <unordered_map>

namespace dorm_energy::detection
{

    class RoomStateAggregator
    {
    public:
        std::optional<core::RoomState> update(
            const core::SensorReading &reading);

        const std::deque<core::RoomState> &getHistory(
            const std::string &deviceId) const;

        const std::unordered_map<
            std::string,
            core::RoomState> &
        getCurrentStates() const;

    private:
        std::unordered_map<
            std::string,
            core::RoomState>
            states_;

        std::unordered_map<
            std::string,
            std::deque<core::RoomState>>
            history_;
    };

}