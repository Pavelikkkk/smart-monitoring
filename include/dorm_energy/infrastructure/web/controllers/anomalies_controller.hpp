#pragma once

#include "dorm_energy/infrastructure/web/context/web_context.hpp"

namespace dorm_energy::web
{
    void registerAnomalyRoutes(const WebContext &context);
} // namespace dorm_energy::web
