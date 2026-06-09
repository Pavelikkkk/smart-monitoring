# ============================================
# Основная библиотека проекта
# ============================================

add_library(dorm_energy_core STATIC
    # Core
    src/dorm_energy/core/measurement.cpp
    src/dorm_energy/core/alert_severity.cpp

    # Infrastructure
    src/dorm_energy/infrastructure/logging/spdlog_logger.cpp
    src/dorm_energy/infrastructure/simulation/synthetic_data_generator.cpp
    src/dorm_energy/infrastructure/notifier/console_notifier.cpp
    src/dorm_energy/infrastructure/notifier/telegram_config.cpp      
    src/dorm_energy/infrastructure/notifier/telegram_notifier.cpp
    src/dorm_energy/infrastructure/handlers/message_handler.cpp
    src/dorm_energy/infrastructure/cli/cli_parser.cpp
    src/dorm_energy/infrastructure/mqtt/mqtt_client.cpp
    src/dorm_energy/infrastructure/mqtt/message_parser.cpp
    src/dorm_energy/infrastructure/storage/postgres_repository.cpp
    src/dorm_energy/infrastructure/detection/rule_based_detector.cpp
    src/dorm_energy/infrastructure/notifier/telegram_queue.cpp
    src/dorm_energy/infrastructure/notifier/telegram_poller.cpp
    src/dorm_energy/infrastructure/simulation/csv_exporter.cpp
    src/dorm_energy/infrastructure/detection/room_state_aggregator.cpp
    src/dorm_energy/infrastructure/detection/anomaly_tracker.cpp
    src/dorm_energy/infrastructure/detection/feature_extractor.cpp
    src/dorm_energy/infrastructure/detection/onnx_detector.cpp
    src/dorm_energy/infrastructure/detection/hybrid_detector.cpp
    src/dorm_energy/infrastructure/web/server/web_server.cpp
    src/dorm_energy/infrastructure/auth/jwt_service.cpp
    src/dorm_energy/infrastructure/auth/openssl_password_hasher.cpp


    # Application
    src/dorm_energy/application/config/app_config.cpp
    src/dorm_energy/application/commands/simulate_command.cpp
    src/dorm_energy/application/commands/daemon_command.cpp
    src/dorm_energy/application/application_builder.cpp
    src/dorm_energy/application/application.cpp
    src/dorm_energy/application/runtime.cpp
    src/dorm_energy/application/notifier_service.cpp
    src/dorm_energy/application/auth/auth_service.cpp
    
)

target_include_directories(
    dorm_energy_core
    PRIVATE
        ${ONNXRUNTIME_INCLUDE_DIR}
)

target_link_libraries(dorm_energy_core
    PUBLIC
        CLI11::CLI11
        spdlog::spdlog
        fmt::fmt
        nlohmann_json::nlohmann_json
        libpqxx::pqxx
        OpenSSL::SSL
        OpenSSL::Crypto
        PahoMqttCpp::paho-mqttpp3
        CURL::libcurl
        Drogon::Drogon
        jwt-cpp::jwt-cpp
        ${ONNXRUNTIME_LIB}
)
