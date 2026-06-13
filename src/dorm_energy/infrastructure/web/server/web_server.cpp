#include "dorm_energy/infrastructure/web/server/web_server.hpp"

#include "dorm_energy/infrastructure/web/context/web_context.hpp"
#include "dorm_energy/infrastructure/web/middleware/cors_middleware.hpp"
#include "dorm_energy/infrastructure/web/routes/route_registrar.hpp"

#include <drogon/drogon.h>

#include <iostream>
#include <utility>

namespace dorm_energy::web
{
    WebServer::WebServer(std::shared_ptr<dorm_energy::detection::RoomStateAggregator> aggregator,
                         std::shared_ptr<dorm_energy::storage::IMeasurementRepository> repository,
                         std::shared_ptr<AuthService> authService)
        : aggregator_(std::move(aggregator)), repository_(std::move(repository)),
          authService_(std::move(authService))
    {
    }

    void WebServer::start()
    {
        drogon::app().addListener("0.0.0.0", 8080);

        registerCorsMiddleware();
        registerWebRoutes(WebContext{aggregator_, repository_, authService_});

        std::cout << "Starting Drogon..." << std::endl;

        server_thread_ = std::thread([]() { drogon::app().run(); });
    }

    void WebServer::stop()
    {
        drogon::app().quit();

        if (server_thread_.joinable())
        {
            server_thread_.join();
        }
    }
} // namespace dorm_energy::web
