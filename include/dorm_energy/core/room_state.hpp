#pragma once

#include <chrono>
#include <string>

namespace dorm_energy::core
{

    struct RoomState
    {
        std::string deviceId;

        std::chrono::system_clock::time_point timestamp;

        bool motion{false};

        double power{0.0};

        double light{0.0};
    };

} // namespace dorm_energy::core