#include "dorm_energy/application/notifier_service.hpp"

#include <gtest/gtest.h>

using dorm_energy::application::INotifier;
using dorm_energy::application::NotifierService;
using dorm_energy::core::RoomState;
using dorm_energy::detection::AnomalyInfo;

namespace
{
    class FakeNotifier final : public INotifier
    {
    public:
        explicit FakeNotifier(bool sendResult)
            : sendResult_(sendResult)
        {
        }

        bool sendAlert(const RoomState &, const AnomalyInfo &) override
        {
            ++sendAlertCalls;
            return sendResult_;
        }

        std::size_t sendAlerts(const std::vector<RoomState> &states, const AnomalyInfo &) override
        {
            ++sendAlertsCalls;
            return sendResult_ ? states.size() : 0U;
        }

        int sendAlertCalls{0};
        int sendAlertsCalls{0};

    private:
        bool sendResult_;
    };
}

TEST(NotifierServiceTest, EmptyServiceDoesNotSend)
{
    NotifierService service;

    EXPECT_FALSE(service.sendAlert(RoomState{}, AnomalyInfo{}));
    EXPECT_EQ(service.sendAlerts({RoomState{}}, AnomalyInfo{}), 0U);
}

TEST(NotifierServiceTest, IgnoresNullNotifiers)
{
    NotifierService service;
    service.addNotifier(nullptr);

    EXPECT_FALSE(service.sendAlert(RoomState{}, AnomalyInfo{}));
}

TEST(NotifierServiceTest, SendsAlertToAllNotifiersAndReportsAggregateSuccess)
{
    NotifierService service;
    auto success = std::make_unique<FakeNotifier>(true);
    auto failure = std::make_unique<FakeNotifier>(false);
    auto *successPtr = success.get();
    auto *failurePtr = failure.get();

    service.addNotifier(std::move(success));
    service.addNotifier(std::move(failure));

    EXPECT_FALSE(service.sendAlert(RoomState{}, AnomalyInfo{}));
    EXPECT_EQ(successPtr->sendAlertCalls, 1);
    EXPECT_EQ(failurePtr->sendAlertCalls, 1);
}

TEST(NotifierServiceTest, SumsBatchResultsAcrossNotifiers)
{
    NotifierService service;
    service.addNotifier(std::make_unique<FakeNotifier>(true));
    service.addNotifier(std::make_unique<FakeNotifier>(true));

    EXPECT_EQ(service.sendAlerts({RoomState{}, RoomState{}}, AnomalyInfo{}), 4U);
    EXPECT_EQ(service.sendAlerts({}, AnomalyInfo{}), 0U);
}
