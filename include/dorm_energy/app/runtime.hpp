#pragma once

#include <atomic>
#include <csignal>
#include <functional>

namespace dorm_energy
{

    class Runtime
    {
    public:
        static void init();
        static void stop();
        static bool is_running();

        static void set_on_stop_callback(std::function<void()> callback); 

    private:
        static inline std::atomic<bool> running_{true}; 
        static inline std::function<void()> on_stop_callback_{nullptr};

        static void signal_handler(int signal);
    };

} // namespace dorm_energy