#pragma once

#include "../interfaces.hpp"
#include "../core/measurement.hpp"

namespace dorm_energy::mqtt
{
    class MqttClient : public IMqttClient
    {
    public:
        MqttClient(); 
        ~MqttClient() override;

        MqttClient(const MqttClient &) = delete;
        MqttClient &operator=(const MqttClient &) = delete;

        MqttClient(MqttClient &&) noexcept; 
        MqttClient &operator=(MqttClient &&) noexcept;

        void connect(const std::string &broker = "tcp://127.0.0.1:1883",
                     const std::string &client_id = "dorm-sim") override;

        void subscribe(const std::string &topic = "devices/+/power") override;

        void set_message_callback(MessageCallback callback) override;

        void start_simulation_mode() override;

    private: 
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

} // namespace dorm_energy::mqtt