cmake_minimum_required(VERSION 3.23)

project(dorm-energy-sim
    VERSION 2.0
    DESCRIPTION "Имитация и мониторинг энергопотребления в общежитии"
    LANGUAGES CXX
)

# ============================================
# Глобальные настройки 
# ============================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4 /WX) 
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wshadow -Wnon-virtual-dtor)
endif()

message(STATUS "Compiler warnings enabled with -Werror")

# ============================================
# Зависимости
# ============================================

add_subdirectory(external/CLI11)
add_subdirectory(external/nlohmann_json EXCLUDE_FROM_ALL)

# ============================================
# Основная библиотека проекта
# ============================================

add_library(dorm_energy_core STATIC # возможно нужно разделить
    src/core/measurement.cpp
    src/detection/anomaly_detector.cpp    
    src/simulation/generator.cpp
    src/app/application.cpp
    src/app/commands.cpp
    src/logging/logger.cpp
    src/mqtt/mqtt_client.cpp
    src/mqtt/message_parser.cpp
    src/storage/repository.cpp
    src/app/runtime.cpp
    src/app/message_handler.cpp
    src/app/notifier.cpp
)

target_include_directories(dorm_energy_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================
# Основное приложение
# ============================================

target_link_libraries(dorm_energy_core 
 PUBLIC 
    CLI11::CLI11
    nlohmann_json::nlohmann_json
)

add_executable(dorm-sim
    src/main.cpp
)
target_link_libraries(dorm-sim 
 PRIVATE 
    dorm_energy_core
)

# ============================================
# ТЕСТЫ
# ============================================

option(BUILD_TESTING OFF)

if(BUILD_TESTING)
    enable_testing()
    message(STATUS "Тестирование включено")
endif()

message(STATUS "Project ${PROJECT_NAME} configured successfully")

# message(STATUS "C Compiler ID: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
# message(STATUS "Compiler Path: ${CMAKE_CXX_COMPILER}")