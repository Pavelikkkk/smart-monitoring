// src/mqtt/mqtt_client.cpp

#include <iostream>
#include <format>
#include <thread>
#include <chrono>

#include "dorm_energy/mqtt/mqtt_client.hpp"
#include "dorm_energy/mqtt/message_parser.hpp"

namespace dorm_energy::mqtt
{

    struct MqttClient::Impl
    {
        MessageCallback callback;
        std::thread sim_thread;
        bool simulation_mode = false;
    };

    MqttClient::MqttClient() : impl_(std::make_unique<Impl>()) {}

    MqttClient::~MqttClient()
    {
        if (impl_->sim_thread.joinable())
            impl_->sim_thread.join();
    }

    MqttClient::MqttClient(MqttClient &&) noexcept = default;
    MqttClient &MqttClient::operator=(MqttClient &&) noexcept = default;

    void MqttClient::connect(const std::string &broker, const std::string &client_id)
    {
        std::cout << std::format("[MQTT STUB] Подключение к {} как {}\n", broker, client_id);
        std::cout << "[MQTT STUB] ✅ Успешно подключено (заглушка)\n";
    }

    void MqttClient::subscribe(const std::string &topic)
    {
        std::cout << std::format("[MQTT STUB] Подписка на топик: {}\n", topic);
    }

    void MqttClient::set_message_callback(MessageCallback callback)
    {
        impl_->callback = std::move(callback);
        std::cout << "[MQTT STUB] Callback установлен\n";
    }

    void MqttClient::start_simulation_mode()
    {
        if (impl_->sim_thread.joinable())
            return;

        std::cout << "[MQTT STUB] Запущен режим симуляции (фейковые данные)\n";

        impl_->sim_thread = std::thread([this]()
                                        {
        while (true) {
            core::PowerMeasurement m;
            std::string fake_json = R"({"power_kw": 18.5, "hour": 14})";

            if (MessageParser::parse(fake_json, m))
            {
                if (impl_->callback) {
                    impl_->callback(m);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        } });
    }

} // namespace dorm_energy::mqtt