// include/dorm_energy/mqtt/message_parser.hpp
#pragma once

#include <string>

#include "../core/measurement.hpp"

namespace dorm_energy::mqtt
{

    /**
     * @brief Парсер JSON-сообщений от ESP32
     */
    class MessageParser
    {
    public:
        /**
         * @brief Парсит payload от датчика
         * Ожидаемый формат: {"device_id":"esp01","power_kw":23.45,"timestamp":1740000000}
         */
        static bool parse(const std::string &payload, core::PowerMeasurement &out);

        static std::string to_json(const core::PowerMeasurement &m);
    };

} // namespace dorm_energy::mqttы