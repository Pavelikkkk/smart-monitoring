#include "dorm_energy/infrastructure/web/controllers/analytics_controller.hpp"

#include "dorm_energy/infrastructure/web/middleware/auth_middleware.hpp"
#include "dorm_energy/infrastructure/web/utils/json_response.hpp"

#include <drogon/drogon.h>

namespace dorm_energy::web
{
    void registerAnalyticsRoutes(const WebContext &context)
    {
        auto repository = context.repository;
        auto auth = AuthMiddleware(repository, context.authService);

        drogon::app().registerHandler(
            "/api/analytics/top-consumers",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value result(Json::arrayValue);

                try
                {
                    auto user = auth.requireUser(req);
                    auto consumers = repository->getTopConsumers(10, user.organizationId);

                    for (const auto &consumer : consumers)
                    {
                        Json::Value item;

                        item["roomName"] = consumer.roomName;
                        item["power"] = consumer.power;

                        result.append(item);
                    }

                    callback(makeJsonResponse(result));
                }
                catch (const std::exception &ex)
                {
                    callback(makeJsonResponse(makeErrorJson(ex), statusForError(ex.what())));
                }
            });

        drogon::app().registerHandler(
            "/api/analytics/anomalies-by-type",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value result(Json::arrayValue);

                try
                {
                    auto user = auth.requireUser(req);
                    auto stats = repository->getAnomalyStatistics(user.organizationId);

                    for (const auto &stat : stats)
                    {
                        Json::Value item;

                        item["type"] = stat.type;
                        item["count"] = stat.count;

                        result.append(item);
                    }

                    callback(makeJsonResponse(result));
                }
                catch (const std::exception &ex)
                {
                    callback(makeJsonResponse(makeErrorJson(ex), statusForError(ex.what())));
                }
            });

        drogon::app().registerHandler(
            "/api/analytics/energy-by-room",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value data(Json::arrayValue);

                try
                {
                    auto user = auth.requireUser(req);
                    auto rooms = repository->getEnergyByRoom(user.organizationId);

                    for (const auto &room : rooms)
                    {
                        Json::Value item;

                        item["roomName"] = room.roomName;
                        item["power"] = room.avgPower;

                        data.append(item);
                    }

                    callback(makeJsonResponse(data));
                }
                catch (const std::exception &ex)
                {
                    callback(makeJsonResponse(makeErrorJson(ex), statusForError(ex.what())));
                }
            });

        drogon::app().registerHandler(
            "/api/analytics/severity-distribution",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value data(Json::arrayValue);

                try
                {
                    auto user = auth.requireUser(req);
                    auto stats = repository->getSeverityDistribution(user.organizationId);

                    for (const auto &stat : stats)
                    {
                        Json::Value item;

                        item["severity"] = stat.severity;
                        item["count"] = stat.count;

                        data.append(item);
                    }

                    callback(makeJsonResponse(data));
                }
                catch (const std::exception &ex)
                {
                    callback(makeJsonResponse(makeErrorJson(ex), statusForError(ex.what())));
                }
            });

        drogon::app().registerHandler(
            "/api/power/history",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value history(Json::arrayValue);

                try
                {
                    auto user = auth.requireUser(req);
                    auto points = repository->getPowerHistory(24, user.organizationId);

                    for (const auto &point : points)
                    {
                        Json::Value item;

                        item["time"] = point.time;
                        item["power"] = point.power;

                        history.append(item);
                    }

                    callback(makeJsonResponse(history));
                }
                catch (const std::exception &ex)
                {
                    callback(makeJsonResponse(makeErrorJson(ex), statusForError(ex.what())));
                }
            });
    }
} // namespace dorm_energy::web
