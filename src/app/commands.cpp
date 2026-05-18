// src/app/commands.cpp

#include <iostream>
#include <format>
#include <thread>

#include "dorm_energy/app/commands.hpp"
#include "dorm_energy/mqtt/message_parser.hpp"
#include "dorm_energy/app/application.hpp"
#include "dorm_energy/app/runtime.hpp"

namespace dorm_energy
{
    SimulateCommand::SimulateCommand(
        std::unique_ptr<ILogger> logger,
        std::unique_ptr<IDataGenerator> generator,
        std::unique_ptr<IAnomalyDetector> detector,
        std::unique_ptr<IMeasurementRepository> repository,
        const CLI::App &subcommand)
        : logger_(std::move(logger)), generator_(std::move(generator)), detector_(std::move(detector)), repository_(std::move(repository)), subcommand_(subcommand)
    {
    }

    int SimulateCommand::execute()
    {
        const int days = subcommand_.get_option("--days")->as<int>();
        const bool verbose = subcommand_.get_option("--verbose")->as<bool>();
        const bool inject_anomalies = subcommand_.get_option("--anomalies")->as<bool>();

        logger_->info("Запуск симуляции");

        logger_->info(std::format("Дней: {} | Аномалии: {}", days,
                                  inject_anomalies ? "включены" : "выключены"));

        auto data = generator_->generate();
        int anomaly_count = detector_->detect(data);

        logger_->info(std::format("Симуляция завершена. Измерений: {}, аномалий: {}",
                                  data.size(), anomaly_count));

        if (repository_)
        {
            repository_->save_batch(data);
        }

        if (verbose)
        {
            std::cout << "\nПервые 10 измерений:\n";
            for (size_t i = 0; i < std::min<size_t>(10, data.size()); ++i)
            {
                std::cout << core::to_string(data[i]) << "\n";
            }
            std::cout << "\nОбнаруженные аномалии:\n";
            for (const auto &m : detector_->get_anomalies(data))
            {
                std::cout << core::to_string(m) << "\n";
            }
        }

        return 0;
    }

    DaemonCommand::DaemonCommand(
        std::unique_ptr<ILogger> logger,
        std::unique_ptr<IMqttClient> mqtt_client,
        std::unique_ptr<IAnomalyDetector> detector)
        : logger_(std::move(logger)), 
          mqtt_client_(std::move(mqtt_client)),
          detector_(std::move(detector))
    {
    }

    int DaemonCommand::execute()
    {
        logger_->info("Dorm Energy Simulator запущен в режиме ДЕМОНА");

        Runtime::init();

        auto measurement_handler = [this](const core::PowerMeasurement &m)
        {
            if (detector_->isAnomaly(m))
            {
                logger_->warn(std::format(" АНОМАЛИЯ! {:.2f} кВт в {}:00",
                                          m.power_kw, m.hour_of_day));
            }
            else
            {
                logger_->info(std::format(" Измерение: {:.2f} кВт (час {})",
                                          m.power_kw, m.hour_of_day));
            }
        };

        mqtt_client_->set_message_callback(measurement_handler);
        mqtt_client_->start_simulation_mode();

        logger_->info("Демон запущен. Нажмите Ctrl+C для остановки");

        while (Runtime::is_running())
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        logger_->info("Демон остановлен.");
        return 0;
    }

} // namespace dorm_energy