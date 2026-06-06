#pragma once

#include <string>

namespace dorm_energy::storage
{
    struct DeviceDto
    {
        std::string deviceId;

        std::string deviceName;

        std::string deviceModel;

        std::string firmwareVersion;

        std::string roomName;

        bool isOnline = false;

        std::string lastSeenAt;
    };
}