cmake_minimum_required(VERSION 3.23)

project(dorm-energy-sim
    VERSION 1.1
    DESCRIPTION "Имитация и мониторинг энергопотребления в общежитии"
    LANGUAGES CXX
)

# ============================================
# Глобальные настройки 
# ============================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        add_compile_options(/W4 /WX)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wall -Wextra -Werror -Wpedantic)
    endif()
endif()

# ============================================
# Зависимости
# ============================================

add_subdirectory(external/CLI11)

# ============================================
# Основная библиотека проекта
# ============================================

add_library(dorm_energy_core STATIC
    src/core/measurement.cpp
    src/detection/anomaly_detector.cpp    
    src/simulation/generator.cpp
)

target_include_directories(dorm_energy_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================
# Основное приложение
# ============================================

add_executable(dorm-sim
    src/main.cpp
)

target_link_libraries(dorm-sim 
  PRIVATE
    dorm_energy_core 
    CLI11::CLI11
)

# ============================================
# Санитайзеры
# ============================================

function(enable_sanitizers TARGET_NAME)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(${TARGET_NAME} PRIVATE 
                -fsanitize=address,undefined 
                -g 
                -O1 
                -fno-omit-frame-pointer
            )
            target_link_options(${TARGET_NAME} PRIVATE 
                -fsanitize=address,undefined
            )
            message(STATUS "Sanitizers enabled for ${TARGET_NAME}")
        else()
            message(STATUS "Sanitizers not supported for compiler ${CMAKE_CXX_COMPILER_ID}")
        endif()
    endif()
endfunction()

enable_sanitizers(dorm_energy_core)
enable_sanitizers(dorm-sim)

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