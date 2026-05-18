#pragma once

#include "core/measurement.hpp"

#include <string>
#include <vector>
#include <functional>

namespace dorm_energy
{
    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        virtual void info(const std::string &message) const = 0;
        virtual void debug(const std::string &message) const = 0;
        virtual void warn(const std::string &message) const = 0;
        virtual void error(const std::string &message) const = 0;
    };

    class IMqttClient
    {
    public:
        virtual ~IMqttClient() = default;

        virtual void connect(const std::string &broker = "tcp://127.0.0.1:1883",
                             const std::string &client_id = "dorm-sim") = 0;

        virtual void subscribe(const std::string &topic = "devices/+/power") = 0;

        using MessageCallback = std::function<void(const core::PowerMeasurement &)>;
        virtual void set_message_callback(MessageCallback callback) = 0;

        virtual void start_simulation_mode() = 0;
    };

    class IDataGenerator
    {
    public:
        virtual ~IDataGenerator() = default;
        virtual core::SimulationData generate() const = 0;
    };

    class IAnomalyDetector
    {
    public:
        virtual ~IAnomalyDetector() = default;

        virtual int detect(core::SimulationData &data) const = 0;
        virtual bool isAnomaly(const core::PowerMeasurement &m) const = 0;

        using AnomalyList = std::vector<core::PowerMeasurement>;
        virtual AnomalyList get_anomalies(const core::SimulationData &data) const = 0;
    };

    class IMeasurementRepository
    {
    public:
        virtual ~IMeasurementRepository() = default;

        virtual void save(const core::PowerMeasurement &measurement) = 0;
        virtual void save_batch(const core::SimulationData &data) = 0;
    };

    class IMessageHandler
    {
    public:
        virtual ~IMessageHandler() = default;
        virtual void handle(const core::PowerMeasurement &measurement) = 0;
    };

} // namespace dorm_energy