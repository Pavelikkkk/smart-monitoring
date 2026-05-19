// src/app/message_handler.cpp

#include "dorm_energy/app/message_handler.hpp"
#include <format>

namespace dorm_energy::app
{

    MessageHandler::MessageHandler(
        std::unique_ptr<ILogger> logger,
        std::unique_ptr<IAnomalyDetector> detector,
        std::unique_ptr<INotifier> notifier,
        std::unique_ptr<IMeasurementRepository> repository)
        : logger_(std::move(logger)), detector_(std::move(detector)), notifier_(std::move(notifier)), repository_(std::move(repository))
    {
        if (!logger_ || !detector_ || !notifier_)
            throw std::invalid_argument("MessageHandler: logger, detector и notifier обязательны");
    }

    void MessageHandler::handle(const core::PowerMeasurement &measurement)
    {
        const bool is_anomaly = detector_->isAnomaly(measurement);

        std::string log_message = std::format(
            "Измерение: {:.2f} кВт (час {:02d})",
            measurement.power_kw,
            measurement.hour_of_day);

        if (is_anomaly)
        {
            log_message += " АНОМАЛИЯ ОБНАРУЖЕНА!";
            logger_->warn(log_message);
            notifier_->send_alert(measurement, "Превышение порога мощности");
        }
        else
        {
            logger_->info(log_message);
        }

        batch_buffer_.push_back(measurement);

        if (batch_buffer_.size() >= BATCH_SIZE)
        {
            if (repository_)
                repository_->save_batch(batch_buffer_);

            batch_buffer_.clear();
        }
    }

} // namespace dorm_energy::app