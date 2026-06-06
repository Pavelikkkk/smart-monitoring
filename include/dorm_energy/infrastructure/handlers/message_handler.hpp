// include/dorm_energy/infrastructure/handlers/message_handler.hpp
#pragma once

#include "dorm_energy/domain/detection/istate_detector.hpp"
#include "dorm_energy/infrastructure/detection/room_state_aggregator.hpp"
#include "dorm_energy/domain/storage/imeasurement_repository.hpp"
#include "dorm_energy/application/inotifier.hpp"
#include "dorm_energy/application/imessage_handler.hpp"
#include "dorm_energy/core/measurement.hpp"
#include "dorm_energy/infrastructure/detection/rule_based_detector.hpp"
#include "dorm_energy/infrastructure/detection/anomaly_tracker.hpp"

namespace dorm_energy::handlers
{

    class MessageHandler : public application::IMessageHandler
    {
    public:
        explicit MessageHandler(
            std::unique_ptr<dorm_energy::detection::IStateDetector> detector,
            std::shared_ptr<dorm_energy::storage::IMeasurementRepository> repository,
            std::unique_ptr<dorm_energy::application::INotifier> notifier,
            std::shared_ptr<dorm_energy::detection::RoomStateAggregator> aggregator);

        bool handle(const core::SensorReading &reading) override;
        std::size_t handleBatch(const std::vector<core::SensorReading> &readings) override;

        void flush() override;

    private:
        std::unique_ptr<dorm_energy::detection::IStateDetector> detector_;
        std::shared_ptr<dorm_energy::storage::IMeasurementRepository> repository_;
        std::unique_ptr<dorm_energy::application::INotifier> notifier_;
        std::shared_ptr<dorm_energy::detection::RoomStateAggregator> aggregator_;
        detection::AnomalyTracker tracker_;

        void persistCurrentBatch();

        core::ReadingsBatch batch_;
    };

} // namespace dorm_energy::handlers