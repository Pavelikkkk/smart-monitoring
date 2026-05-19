#pragma once

#include "../interfaces.hpp"
#include "../core/measurement.hpp"

namespace dorm_energy::app
{
    class ConsoleNotifier : public INotifier
    {
    public:
        explicit ConsoleNotifier(std::unique_ptr<ILogger> logger);

        void send_alert(const core::PowerMeasurement& measurement, 
                        const std::string& reason = "") override;

    private:
        std::unique_ptr<ILogger> logger_;
    };
}