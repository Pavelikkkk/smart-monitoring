cmake_minimum_required(VERSION 3.23)

project(dorm-energy-sim
    VERSION 2.0
    DESCRIPTION "Имитация и мониторинг энергопотребления в общежитии"
    LANGUAGES CXX
)

# ============================================
# Модули
# ============================================

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(options)
include(dependencies)
include(core_library)
include(executable)
include(tests)

# ============================================
# Дополнительно
# ============================================

message(STATUS "Project '${PROJECT_NAME}' configured successfully")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
