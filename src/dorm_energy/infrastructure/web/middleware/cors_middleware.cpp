#include "dorm_energy/infrastructure/web/middleware/cors_middleware.hpp"

#include <drogon/drogon.h>

namespace dorm_energy::web
{
    void registerCorsMiddleware()
    {
        drogon::app().registerPreRoutingAdvice(
            [](const drogon::HttpRequestPtr &req, drogon::AdviceCallback &&acb,
               drogon::AdviceChainCallback &&ccb)
            {
                auto response = drogon::HttpResponse::newHttpResponse();

                response->addHeader("Access-Control-Allow-Origin", "*");
                response->addHeader("Access-Control-Allow-Methods",
                                    "GET, POST, PUT, DELETE, OPTIONS");
                response->addHeader("Access-Control-Allow-Headers", "*");

                if (req->method() == drogon::Options)
                {
                    acb(response);
                    return;
                }

                ccb();
            });
    }
} // namespace dorm_energy::web
