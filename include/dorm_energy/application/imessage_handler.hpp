// include/dorm_energy/application/imessage_handler.hpp
#pragma once

#include "dorm_energy/core/measurement.hpp"

namespace dorm_energy::application
{

    /**
     * @brief Интерфейс для обработки входящих MQTT-сообщений
     */
    class IMessageHandler
    {
    public:
        virtual ~IMessageHandler() = default;

        /**
         * @brief Обрабатывает одно новое показание от датчика
         * @param reading полученное показание
         */
        virtual bool handle(const core::SensorReading &reading) = 0;

        /**
         * @brief Принудительно сбрасывает накопленный батч (например, при остановке)
         */
        virtual void flush() = 0;

        /**
         * @brief Обрабатывает пакет новых показаний от датчиков
         * @param readings вектор полученных показаний
         */
        virtual std::size_t handleBatch(const std::vector<core::SensorReading> &readings) = 0;
    };

} // namespace dorm_energy::application