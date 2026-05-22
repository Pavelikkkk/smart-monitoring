// include/dorm_energy/application/application.hpp
#pragma once

#include "dorm_energy/application/config/app_config.hpp"
#include "dorm_energy/infrastructure/cli/cli_parser.hpp"
#include "dorm_energy/application/commands/simulate_command.hpp"
#include "dorm_energy/application/commands/daemon_command.hpp"

#include <memory>

namespace dorm_energy::application
{

    class Application
    {
    public:
        explicit Application(
            AppConfig config,
            std::unique_ptr<cli::CliParser> cliParser,
            std::unique_ptr<SimulateCommand> simulateCommand,
            std::unique_ptr<DaemonCommand> daemonCommand);

        ~Application() = default;

        int run(int argc, char **argv);

    private:
        AppConfig config_;

        std::unique_ptr<cli::CliParser> cliParser_;
        std::unique_ptr<SimulateCommand> simulateCommand_;
        std::unique_ptr<DaemonCommand> daemonCommand_;
    };

} // namespace dorm_energy::application