#include "dorm_energy/core/alert_severity.hpp"
#include <string>

namespace dorm_energy::core
{
    std::string toString(AlertSeverity severity)
    {
        switch (severity)
        {
        case AlertSeverity::Info:
            return "INFO";
        case AlertSeverity::Warning:
            return "WARNING";
        case AlertSeverity::Critical:
            return "CRITICAL";
        default:
            return "UNKNOWN";
        }
    }
}