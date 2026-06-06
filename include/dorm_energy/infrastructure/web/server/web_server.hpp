#pragma once

#include "dorm_energy/infrastructure/detection/room_state_aggregator.hpp"
#include "dorm_energy/domain/storage/imeasurement_repository.hpp"

#include <memory>
#include <thread>

namespace dorm_energy::web
{
    class WebServer
    {
    public:
        WebServer(
            std::shared_ptr<
                dorm_energy::detection::RoomStateAggregator> aggregator,
            std::shared_ptr<
                dorm_energy::storage::IMeasurementRepository> repository);

        void start();
        void stop();

    private:
        std::shared_ptr<
            dorm_energy::detection::RoomStateAggregator>
            aggregator_;

        std::shared_ptr<
            dorm_energy::storage::IMeasurementRepository>
            repository_;

        std::thread server_thread_;
    };
}