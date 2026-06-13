#include "dorm_energy/application/runtime.hpp"

#include <csignal>
#include <iostream>

namespace dorm_energy::application
{

    void Runtime::init()
    {
        std::signal(SIGINT, Runtime::signalHandler);
        std::signal(SIGTERM, Runtime::signalHandler);

        // std::signal(SIGPIPE, SIG_IGN);

        running_.store(true);
    }

    void Runtime::signalHandler(int signal)
    {
        std::cout << "\nReceived signal " << signal << ". Initiating graceful shutdown...\n";

        running_.store(false);

        if (on_stop_callback_)
        {
            on_stop_callback_();
        }
    }

    bool Runtime::isRunning() noexcept
    {
        return running_.load();
    }

    void Runtime::setOnStopCallback(std::function<void()> callback)
    {
        on_stop_callback_ = std::move(callback);
    }

    void Runtime::stop()
    {
        running_.store(false);
        if (on_stop_callback_)
        {
            on_stop_callback_();
        }
    }

} // namespace dorm_energy::application