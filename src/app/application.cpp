// src/app/application.cpp
#include <iostream>
#include <format>
#include <clocale>
#include <thread>

#include "dorm_energy/app/application.hpp" 

namespace dorm_energy
{

    Application::Application( 
        std::unique_ptr<ILogger> logger,
        std::unique_ptr<IMqttClient> mqtt_client,
        std::unique_ptr<IDataGenerator> generator,
        std::unique_ptr<IAnomalyDetector> detector,
        std::unique_ptr<IMeasurementRepository> repository)
        : logger_(std::move(logger)),
          mqtt_client_(std::move(mqtt_client)),
          generator_(std::move(generator)),
          detector_(std::move(detector)),
          repository_(std::move(repository))
    {
    }

    int Application::run(int argc, char** argv)
    {
        std::setlocale(LC_ALL, "ru_RU.UTF-8");

        CLI::App app{"Dorm Energy Simulator — Имитация энергопотребления в общежитии"};

        bool version_flag{false};
        app.add_flag("--version,-v", version_flag, "Показать версию");

        auto *simulate = app.add_subcommand("simulate", "Запустить симуляцию");
        int days = 30;
        simulate->add_option("--days,-d", days)->default_val(30)->check(CLI::Range(1, 365));
        simulate->add_flag("--verbose,-v", "Подробный вывод");
        simulate->add_flag("--anomalies", "Включить аномалии");

        auto *daemon = app.add_subcommand("daemon", "Запустить в режиме демона");

        try
        {
            app.parse(argc, argv);
        }
        catch (const CLI::ParseError &e)
        {
            return app.exit(e);
        }

        if (version_flag)
        {
            std::cout << "Dorm Energy Simulator v2.0\nАвтор: Pasha\nДата сборки: " << __DATE__ << "\n";
            return 0;
        }

        if (simulate->parsed())
        {
            SimulateCommand cmd( 
                std::move(logger_),
                std::move(generator_),
                std::move(detector_),
                std::move(repository_),
                *simulate);
            return cmd.execute();
        }

        if (daemon->parsed())
        {
            DaemonCommand cmd(std::move(logger_), std::move(mqtt_client_), std::move(detector_));
            return cmd.execute();
        }

        std::cout << app.help() << "\n";
        return 1;
    }

} // namespace dorm_energy