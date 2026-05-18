// src/app/runtime.cpp

#include "dorm_energy/app/runtime.hpp"
#include <iostream>

namespace dorm_energy
{

    void Runtime::init()
    {
        running_ = true;
        std::signal(SIGINT, Runtime::signal_handler);
        std::signal(SIGTERM, Runtime::signal_handler);
    }

    void Runtime::stop()
    {
        running_ = false;
        if (on_stop_callback_)
        {
            on_stop_callback_();
        }
    }

    bool Runtime::is_running()
    {
        return running_;
    }

    void Runtime::set_on_stop_callback(std::function<void()> callback)
    {
        on_stop_callback_ = std::move(callback);
    }

    void Runtime::signal_handler(int)
    {
        std::cout << "\n\n Получен сигнал завершения. Останавливаем приложение...\n";
        stop();
    }

} // namespace dorm_energy