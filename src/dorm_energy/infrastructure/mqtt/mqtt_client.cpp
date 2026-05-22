// src/mqtt/mqtt_client.cpp
#include <thread>
#include <chrono>
#include <atomic>
#include <format>

#include "dorm_energy/mqtt/mqtt_client.hpp"
#include "dorm_energy/app/message_handler.hpp"
#include "dorm_energy/simulation/generator.hpp"
#include "dorm_energy/mqtt/message_parser.hpp" // как он его видит?

namespace dorm_energy::mqtt
{

    struct MqttClient::Impl
    {
        std::unique_ptr<IMessageHandler> handler_;
        std::unique_ptr<simulation::SyntheticDataGenerator> generator_;
        MqttMode mode_ = MqttMode::Simulation;

        // Симуляция
        core::SimulationData simulated_data_;
        size_t current_index_{0};

        // Реальный MQTT
        // mqtt::async_client client_;
        // mqtt::connect_options conn_opts_;

        std::unique_ptr<std::thread> worker_thread_;
        std::atomic<bool> running_{false};
        IMqttClient::MessageCallback callback_;
    };

    MqttClient::MqttClient(std::unique_ptr<simulation::SyntheticDataGenerator> generator)
        : impl_(std::make_unique<Impl>())
    {
        if (generator)
            impl_->generator_ = std::move(generator);
        else
            impl_->generator_ = std::make_unique<simulation::SyntheticDataGenerator>();

        impl_->simulated_data_ = impl_->generator_->generate();
    }

    MqttClient::~MqttClient()
    {
        stop();
    }

    void MqttClient::set_mode(MqttMode mode)
    {
        impl_->mode_ = mode;
        // TODO: если переключаем на Real — переподключаемся
    }

    void MqttClient::set_handler(std::unique_ptr<IMessageHandler> handler)
    {
        impl_->handler_ = std::move(handler);
    }

    void MqttClient::set_message_callback(MessageCallback callback)
    {
        impl_->callback_ = std::move(callback);
    }

    void MqttClient::connect(const std::string & /*broker*/, const std::string & /*client_id*/)
    {
        // TODO: Реализация настоящего подключения
    }

    void MqttClient::subscribe(const std::string & /*topic*/)
    {
        // TODO
    }

    void MqttClient::start()
    {
        if (impl_->running_)
            return;

        impl_->running_ = true;
        impl_->worker_thread_ = std::make_unique<std::thread>([this]()
                                                              {
        while (impl_->running_)
        {
            core::PowerMeasurement m;

            if (impl_->mode_ == MqttMode::Simulation)
            {
                if (impl_->current_index_ >= impl_->simulated_data_.size())
                    impl_->current_index_ = 0;

                m = impl_->simulated_data_[impl_->current_index_++];
                m.timestamp = core::Clock::now();
            }
            else
            {
                // TODO: Реальный приём из MQTT
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }

            if (impl_->handler_)
                impl_->handler_->handle(m);
            else if (impl_->callback_)
                impl_->callback_(m);

            std::this_thread::sleep_for(std::chrono::seconds(3));
        } });
    }

    void MqttClient::stop()
    {
        if (!impl_->running_)
            return;
        impl_->running_ = false;
        if (impl_->worker_thread_ && impl_->worker_thread_->joinable())
            impl_->worker_thread_->join();
    }

} // namespace dorm_energy::mqtt