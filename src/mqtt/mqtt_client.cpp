// src/mqtt/mqtt_client.cpp
#include <thread>
#include <chrono>
#include <atomic>

#include "dorm_energy/mqtt/mqtt_client.hpp"
#include "dorm_energy/app/message_handler.hpp"
#include "dorm_energy/interfaces.hpp"
#include "dorm_energy/simulation/generator.hpp"

namespace dorm_energy::mqtt
{

    struct MqttClient::Impl
    {
        std::unique_ptr<IMessageHandler> handler_;
        std::unique_ptr<std::thread> worker_thread_;
        std::unique_ptr<simulation::SyntheticDataGenerator> generator_;
        core::SimulationData simulated_data_;
        std::atomic<bool> running_{false};
        size_t current_index_{0};
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

    void MqttClient::set_message_callback(dorm_energy::IMqttClient::MessageCallback callback)
    {
        impl_->callback_ = std::move(callback);
    }

    void MqttClient::set_handler(std::unique_ptr<IMessageHandler> handler)
    {
        impl_->handler_ = std::move(handler);
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
            if (impl_->current_index_ >= impl_->simulated_data_.size())
                impl_->current_index_ = 0; 

            core::PowerMeasurement m = impl_->simulated_data_[impl_->current_index_++];
            m.timestamp = core::Clock::now();

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

    void MqttClient::connect(const std::string &, const std::string &)
    {
        // TODO
    }

    void MqttClient::subscribe(const std::string &)
    {
        // TODO
    }

} // namespace