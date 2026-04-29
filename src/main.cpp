// main.cpp

#include <iostream>
#include <string>
#include <string_view>
#include <format>           
#include <CLI/CLI.hpp> 
#include <clocale>

#include "dorm_energy/simulation/generator.hpp"
#include "dorm_energy/detection/anomaly_detector.hpp"
#include "dorm_energy/core/measurement.hpp"    

int main(int argc, char* argv[]){

    std::setlocale(LC_ALL, "ru_RU.UTF-8");
   
    CLI::App app{"Dorm Energy Simulator — Имитация энергопотребления в общежитии"};
    
    // ────────────────────────────────────────────────────────────────
    // Глобальный флаг версии
    // ────────────────────────────────────────────────────────────────

    bool version_flag{false};
    app.add_flag("--version, -v", version_flag, "Показать версию программы");

    // ────────────────────────────────────────────────────────────────
    // Подкоманда: simulate
    // ────────────────────────────────────────────────────────────────
    auto* simulate = app.add_subcommand("simulate", "Запустить симуляцию энергопотребления");  

    int days{30}; // default 
    simulate->add_option("--days,-d", days, "Количество дней симуляции")
            ->default_val(30)
            ->check(CLI::Range(1, 365, "days"));

    bool verbose{false};
    simulate->add_flag("--verbose,-v", verbose, "Подробный вывод");
    
    bool inject_anomalies = false;
    simulate->add_flag("--anomalies", inject_anomalies, "Включить искусственные аномалии");
    
    std::string output_mode = "console";
    simulate->add_option("--output", output_mode, "Режим вывода (console)")
            ->default_val("console");
    
    // ────────────────────────────────────────────────────────────────
    // Парсинг аргументов
    // ────────────────────────────────────────────────────────────────

    try {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError& e) {
        return app.exit(e);
    }
    
    // ────────────────────────────────────────────────────────────────
    // Обработка флага версии
    // ────────────────────────────────────────────────────────────────
    
    if (version_flag) {
        std::cout << "Dorm Energy Simulator v1.1\n"
                  << "Автор: Pasha\n"
                  << "Дата сборки: " << __DATE__ << "\n";
        return 0;
    }

    // ────────────────────────────────────────────────────────────────
    // Основная логика подкоманды simulate
    // ────────────────────────────────────────────────────────────────
    
    if (simulate->parsed()) {
        std::cout << "Запуск симуляции энергопотребления...\n";
        std::cout << std::format("Дней: {} | Аномалии: {}\n", days, inject_anomalies ? "включены" : "выключены");

        // Настройка генератора
        dorm_energy::simulation::GeneratorConfig gen_config;
        gen_config.days = days;
        gen_config.inject_anomalies = inject_anomalies;

        dorm_energy::simulation::SyntheticDataGenerator generator{gen_config};
        auto data = generator.generate();

        // Детекция аномалий
        dorm_energy::detection::AnomalyDetectorConfig det_config;
        dorm_energy::detection::AnomalyDetector detector{det_config};

        int anomaly_count = detector.detect(data);

        // Вывод результатов
        std::cout << std::format("\nСимуляция завершена!\n");
        std::cout << std::format("Сгенерировано измерений: {}\n", data.size());
        std::cout << std::format("Обнаружено аномалий: {}\n", anomaly_count);

        if (verbose) {
            std::cout << "\nПервые 10 измерений:\n";
            for (size_t i = 0; i < std::min<size_t>(10, data.size()); ++i) {
                std::cout << dorm_energy::core::to_string(data[i]) << "\n";
            }
        }

        return 0;
    }

    std::cout << app.help() << "\n";
    return 1;
}