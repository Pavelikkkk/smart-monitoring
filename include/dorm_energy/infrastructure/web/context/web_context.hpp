#pragma once

#include "dorm_energy/application/auth/auth_service.hpp"
#include "dorm_energy/domain/storage/imeasurement_repository.hpp"
#include "dorm_energy/infrastructure/detection/room_state_aggregator.hpp"

#include <memory>

namespace dorm_energy::web
{
    struct WebContext
    {
        std::shared_ptr<dorm_energy::detection::RoomStateAggregator> aggregator;
        std::shared_ptr<dorm_energy::storage::IMeasurementRepository> repository;
        std::shared_ptr<AuthService> authService;
    };
} // namespace dorm_energy::web
