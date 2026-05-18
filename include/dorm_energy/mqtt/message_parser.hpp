#pragma once

#include "../core/measurement.hpp"

namespace dorm_energy::mqtt 
{
    /**
     * @brief Парсер JSON-сообщений от IoT-датчиков (ESP32)
     */
    class MessageParser
    {
    public:
        MessageParser() = delete; 
        MessageParser(const MessageParser &) = delete;
        MessageParser &operator=(const MessageParser &) = delete;

        /**
         * @brief Парсит JSON payload от датчика
         * @param payload JSON-строка
         * @param out     Выходной объект 
         * @return true, если парсинг успешен
         */
        static bool parse(const std::string &payload, core::PowerMeasurement &out);

        /**
         * @brief Преобразует измерение в JSON для отправки
         */
        static std::string to_json(const core::PowerMeasurement &m);
    };

} // namespace dorm_energy::mqtt