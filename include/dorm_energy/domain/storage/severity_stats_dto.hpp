#pragma once

#include <string>

namespace dorm_energy::storage
{
    struct SeverityStatsDto
    {
        std::string severity;
        int count{};
    };
}