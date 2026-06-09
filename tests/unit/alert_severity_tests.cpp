#include "dorm_energy/core/alert_severity.hpp"

#include <gtest/gtest.h>

using dorm_energy::core::AlertSeverity;

TEST(AlertSeverityTest, ConvertsKnownSeveritiesToStableStrings)
{
    EXPECT_EQ(dorm_energy::core::toString(AlertSeverity::Info), "INFO");
    EXPECT_EQ(dorm_energy::core::toString(AlertSeverity::Warning), "WARNING");
    EXPECT_EQ(dorm_energy::core::toString(AlertSeverity::Critical), "CRITICAL");
}

TEST(AlertSeverityTest, ConvertsUnknownEnumValueToUnknown)
{
    EXPECT_EQ(dorm_energy::core::toString(static_cast<AlertSeverity>(42)), "UNKNOWN");
}
