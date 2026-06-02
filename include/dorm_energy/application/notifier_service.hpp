#pragma once

#include "dorm_energy/application/inotifier.hpp"
#include "dorm_energy/core/room_state.hpp"
#include "dorm_energy/domain/detection/anomaly_info.hpp"
#include <vector>
#include <memory>

namespace dorm_energy::application
{
    /**
     * @brief Композитный нотификатор — позволяет использовать несколько каналов одновременно
     * (Console + Telegram + WebSocket и т.д.)
     */
    class NotifierService : public INotifier
    {
    public:
        NotifierService() = default;

        void addNotifier(std::unique_ptr<INotifier> notifier);

        bool sendAlert(const core::RoomState &state,
                       const detection::AnomalyInfo &info) override;

        std::size_t sendAlerts(const std::vector<core::RoomState> &states,
                               const detection::AnomalyInfo &info) override;

    private:
        std::vector<std::unique_ptr<INotifier>> notifiers_;
    };
}