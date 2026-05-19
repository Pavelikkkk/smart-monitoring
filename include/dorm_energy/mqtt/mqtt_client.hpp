#pragma once

#include "../interfaces.hpp"
#include "../core/measurement.hpp"
#include "../simulation/generator.hpp"

namespace dorm_energy::mqtt
{
    class MqttClient : public IMqttClient
    {
    public:
        explicit MqttClient(std::unique_ptr<simulation::SyntheticDataGenerator> generator = nullptr);
        MqttClient();
        ~MqttClient() override;

        MqttClient(const MqttClient &) = delete;
        MqttClient &operator=(const MqttClient &) = delete;

        MqttClient(MqttClient &&) noexcept;
        MqttClient &operator=(MqttClient &&) noexcept;

        void connect(const std::string &broker = "tcp://127.0.0.1:1883",
                     const std::string &client_id = "dorm-sim") override;

        void subscribe(const std::string &topic = "devices/+/power") override;

        virtual void start() override;
        virtual void stop() override;

        void set_handler(std::unique_ptr<IMessageHandler> handler) override;

        void set_message_callback(MessageCallback callback) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
        std::unique_ptr<IMessageHandler> handler_;
        std::unique_ptr<simulation::SyntheticDataGenerator> generator_;
        core::SimulationData simulated_data_;
        size_t current_index_{0};
    };

} // namespace dorm_energy::mqtt