#include "dorm_energy/infrastructure/web/controllers/anomalies_controller.hpp"

#include "dorm_energy/infrastructure/web/middleware/auth_middleware.hpp"
#include "dorm_energy/infrastructure/web/utils/json_response.hpp"

#include <drogon/drogon.h>

namespace dorm_energy::web
{
    void registerAnomalyRoutes(const WebContext &context)
    {
        auto repository = context.repository;
        auto auth = AuthMiddleware(repository, context.authService);

        drogon::app().registerHandler(
            "/api/anomalies/latest",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value anomalies(Json::arrayValue);

                try
                {
                    auto user = auth.requireUser(req);
                    auto latest = repository->getLatestAnomalies(20, user.organizationId);

                    for (const auto &anomaly : latest)
                    {
                        Json::Value item;

                        item["room"] = anomaly.room;
                        item["type"] = anomaly.type;
                        item["severity"] = anomaly.severity;
                        item["score"] = anomaly.score;
                        item["description"] = anomaly.description;
                        item["detectedAt"] = anomaly.detectedAt;

                        anomalies.append(item);
                    }

                    callback(makeJsonResponse(anomalies));
                }
                catch (const std::exception &ex)
                {
                    callback(makeJsonResponse(makeErrorJson(ex), statusForError(ex.what())));
                }
            });
    }
} // namespace dorm_energy::web
