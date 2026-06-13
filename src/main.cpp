#include "dorm_energy/application/application.hpp"
#include "dorm_energy/application/application_builder.hpp"
#include "dorm_energy/application/config/app_config.hpp"

#include <iostream>

int main(int argc, char **argv)
{

    std::cout << "=== Dorm Energy Simulator ===\n" << std::endl;

    try
    {
        dorm_energy::application::AppConfig config =
            dorm_energy::application::AppConfig::loadFromEnvironment();

        auto application = dorm_energy::application::ApplicationBuilder()
                               .withConfig(std::move(config))
                               .build();

        return application->run(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred!" << std::endl;
        return 1;
    }
}