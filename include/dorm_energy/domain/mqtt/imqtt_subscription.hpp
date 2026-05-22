// include/dorm_energy/domain/mqtt/imqtt_subscription.hpp
#pragma once

#include <string>

namespace dorm_energy::mqtt
{

    /**
     * @brief Интерфейс для управления подписками на MQTT-топики
     */
    class IMqttSubscription
    {
    public:
        virtual ~IMqttSubscription() = default;

        /**
         * @brief Подписывается на топик MQTT
         * @param topic пример: "devices/+/power" или "devices/kitchen/power"
         */
        virtual void subscribe(const std::string &topic = "devices/+/power") = 0;
    };

} // namespace dorm_energy::domain::mqtt