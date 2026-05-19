// include/dorm_energy/app/message_handler.hpp
#pragma once

#include "../interfaces.hpp"
#include "../core/measurement.hpp"
#include <vector>

namespace dorm_energy::app
{
    class MessageHandler : public IMessageHandler
    {
    public:
        explicit MessageHandler(
            std::unique_ptr<ILogger> logger,
            std::unique_ptr<IAnomalyDetector> detector,
            std::unique_ptr<INotifier> notifier,
            std::unique_ptr<IMeasurementRepository> repository);

        void handle(const core::PowerMeasurement &measurement) override;

    private:
        std::unique_ptr<ILogger> logger_;
        std::unique_ptr<IAnomalyDetector> detector_;
        std::unique_ptr<INotifier> notifier_;
        std::unique_ptr<IMeasurementRepository> repository_;

        std::vector<core::PowerMeasurement> batch_buffer_;
        static constexpr size_t BATCH_SIZE = 10;
    };

} // namespace dorm_energy::app