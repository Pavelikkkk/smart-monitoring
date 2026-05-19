#include "dorm_energy/app/notifier.hpp"
#include <format>

namespace dorm_energy::app
{

ConsoleNotifier::ConsoleNotifier(std::unique_ptr<ILogger> logger)
    : logger_(std::move(logger))
{
}

void ConsoleNotifier::send_alert(const core::PowerMeasurement& measurement, 
                                 const std::string& reason)
{
    std::string alert = std::format(
        " АНОМАЛИЯ! {:.2f} кВт (час {:02d}) — {}", 
        measurement.power_kw, 
        measurement.hour_of_day,
        reason.empty() ? "превышение порога" : reason
    );

    logger_->warn(alert);
    // TODO: позже Tg, email и т д
}

} // namespace dorm_energy::app