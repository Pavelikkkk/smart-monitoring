// src/app/commands.cpp

#include <iostream>
#include <format>
#include <thread>

#include "dorm_energy/app/commands.hpp"
#include "dorm_energy/app/message_handler.hpp"
#include "dorm_energy/app/notifier.hpp"
#include "dorm_energy/mqtt/message_parser.hpp"
#include "dorm_energy/app/application.hpp"
#include "dorm_energy/app/runtime.hpp"
#include "dorm_energy/logging/logger.hpp"
#include "dorm_energy/detection/anomaly_detector.hpp"

namespace dorm_energy::app
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
        std::unique_ptr<IMeasurementRepository> repository)
        : logger_(std::move(logger)),
          mqtt_client_(std::move(mqtt_client)),
          repository_(std::move(repository))
    {
    }

    int DaemonCommand::execute()
    {
        logger_->info("Демон запущен. Нажмите Ctrl+C для остановки");

        auto handler = std::make_unique<app::MessageHandler>(
            std::make_unique<logging::Logger>("message-handler"),
            std::make_unique<detection::AnomalyDetector>(),
            std::make_unique<app::ConsoleNotifier>(
                std::make_unique<logging::Logger>("notifier")),
            std::move(repository_));

        mqtt_client_->set_handler(std::move(handler));
        mqtt_client_->start();

        Runtime::set_on_stop_callback([this]()
                                      { mqtt_client_->stop(); });

        while (Runtime::is_running())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        logger_->info("Демон остановлен.");
        return 0;
    }

} // namespace dorm_energy