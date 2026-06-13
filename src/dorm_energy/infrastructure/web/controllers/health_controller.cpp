#include "dorm_energy/infrastructure/web/controllers/health_controller.hpp"

#include "dorm_energy/infrastructure/web/utils/json_response.hpp"

#include <drogon/drogon.h>

namespace dorm_energy::web
{
    void registerHealthRoutes()
    {
        drogon::app().registerHandler(
            "/api/health",
            [](const drogon::HttpRequestPtr &,
               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;

                json["mqtt"] = true;
                json["database"] = true;
                json["telegram"] = true;
                json["detector"] = true;

                callback(makeJsonResponse(json));
            });
    }
} // namespace dorm_energy::web
