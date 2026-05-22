// include/dorm_energy/domain/mqtt/imqtt_message_dispatcher.hpp
#pragma once

#include <functional>

#include "dorm_energy/application/imessage_handler.hpp"
#include "dorm_energy/core/measurement.hpp"


namespace dorm_energy::mqtt
{

    /**
     * @brief Интерфейс для диспетчеризации входящих MQTT-сообщений
     *
     * Отвечает за передачу полученных сообщений в обработчик.
     */
    class IMqttMessageDispatcher
    {
    public:
        virtual ~IMqttMessageDispatcher() = default;

        /**
         * @brief Устанавливает обработчик входящих сообщений
         */
        virtual void setHandler(std::unique_ptr<application::IMessageHandler> handler) = 0;

        /**
         * @brief Альтернативный способ — прямой callback
         */
        using MessageCallback = std::function<void(const core::SensorReading &)>;
        virtual void setMessageCallback(MessageCallback callback) = 0;
    };

} // namespace dorm_energy::domain::mqtt