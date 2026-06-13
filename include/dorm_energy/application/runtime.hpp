// include/dorm_energy/application/runtime.hpp
#pragma once

#include <atomic>
#include <csignal>
#include <functional>

namespace dorm_energy::application
{

    /**
     * @brief Управление жизненным циклом приложения
     *
     * Отвечает за:
     * - graceful shutdown при получении сигналов (SIGINT, SIGTERM)
     * - проверку, работает ли приложение
     * - вызов callback'а при остановке
     */
    class Runtime
    {
    public:
        /**
         * @brief Инициализация runtime (установка обработчиков сигналов)
         */
        static void init();

        /**
         * @brief Корректно останавливает приложение
         */
        static void stop();

        /**
         * @brief Проверяет, должно ли приложение продолжать работу
         */
        static bool isRunning() noexcept;

        /**
         * @brief Устанавливает callback, который будет вызван при остановке
         *        (например, для flush() батчей, закрытия соединений и т.д.)
         */
        static void setOnStopCallback(std::function<void()> callback);

    private:
        static inline std::atomic<bool> running_{true};
        static inline std::function<void()> on_stop_callback_{nullptr};

        static void signalHandler(int signal);
    };

} // namespace dorm_energy::application