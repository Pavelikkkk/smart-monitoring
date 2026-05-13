// include/dorm_energy/mqtt/mqtt_client.hpp
#pragma once

#include <string>
#include <functional>
#include <memory>

#include "dorm_energy/core/measurement.hpp"

namespace dorm_energy::mqtt
{

    class MqttClient
    {
    public:
        using MessageCallback = std::function<void(const core::PowerMeasurement &)>;

        MqttClient();
        ~MqttClient();

        MqttClient(const MqttClient &) = delete;
        MqttClient &operator=(const MqttClient &) = delete;
        MqttClient(MqttClient &&) noexcept;
        MqttClient &operator=(MqttClient &&) noexcept;

        // Заглушка вместо реального подключения пока что
        void connect(const std::string &broker = "tcp://127.0.0.1:1883",
                     const std::string &client_id = "dorm-sim");

        void subscribe(const std::string &topic = "devices/+/power");

        void set_message_callback(MessageCallback callback);

        void start_simulation_mode();

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

} // namespace dorm_energy::mqtt
