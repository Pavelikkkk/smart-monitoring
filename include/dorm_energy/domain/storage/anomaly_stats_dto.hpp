#pragma once

#include <string>

namespace dorm_energy::storage
{
    struct AnomalyStatsDto
    {
        std::string type;

        int count{};
    };
}