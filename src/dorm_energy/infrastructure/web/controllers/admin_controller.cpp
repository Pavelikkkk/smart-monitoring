#include "dorm_energy/infrastructure/web/controllers/admin_controller.hpp"

#include "dorm_energy/infrastructure/web/middleware/auth_middleware.hpp"
#include "dorm_energy/infrastructure/web/utils/json_response.hpp"

#include <drogon/drogon.h>

#include <stdexcept>

namespace dorm_energy::web
{
    void registerAdminRoutes(const WebContext &context)
    {
        auto repository = context.repository;
        auto auth = AuthMiddleware(repository, context.authService);

        drogon::app().registerHandler(
            "/api/admin/overview",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auth.requireAdmin(req);
                    json = repository->getAdminOverview();
                    json["success"] = true;
                }
                catch (const std::exception &ex)
                {
                    json = makeErrorJson(ex);
                    status = statusForError(ex.what());
                }

                callback(makeJsonResponse(json, status));
            },
            {drogon::Get});

        drogon::app().registerHandler(
            "/api/admin/buildings",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auth.requireAdmin(req);

                    auto body = req->getJsonObject();

                    if (!body)
                    {
                        throw std::runtime_error("Invalid JSON body");
                    }

                    auto id = repository->createBuildingForOrganization(
                        (*body)["organizationId"].asInt(), (*body)["name"].asString(),
                        (*body)["address"].asString(), (*body)["description"].asString());

                    json["success"] = true;
                    json["id"] = id;
                }
                catch (const std::exception &ex)
                {
                    json = makeErrorJson(ex);
                    status = statusForError(ex.what());
                }

                callback(makeJsonResponse(json, status));
            },
            {drogon::Post});

        drogon::app().registerHandler(
            "/api/admin/rooms",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auth.requireAdmin(req);

                    auto body = req->getJsonObject();

                    if (!body)
                    {
                        throw std::runtime_error("Invalid JSON body");
                    }

                    auto id = repository->createRoomForBuilding(
                        (*body)["buildingId"].asInt(), (*body)["roomName"].asString(),
                        (*body)["roomType"].asString(), (*body)["floorNumber"].asInt());

                    json["success"] = true;
                    json["id"] = id;
                }
                catch (const std::exception &ex)
                {
                    json = makeErrorJson(ex);
                    status = statusForError(ex.what());
                }

                callback(makeJsonResponse(json, status));
            },
            {drogon::Post});

        drogon::app().registerHandler(
            "/api/admin/devices",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auth.requireAdmin(req);

                    auto body = req->getJsonObject();

                    if (!body)
                    {
                        throw std::runtime_error("Invalid JSON body");
                    }

                    repository->createDeviceForRoom(
                        (*body)["deviceId"].asString(), (*body)["deviceName"].asString(),
                        (*body)["deviceModel"].asString(), (*body)["firmwareVersion"].asString(),
                        (*body)["roomId"].asInt());

                    json["success"] = true;
                }
                catch (const std::exception &ex)
                {
                    json = makeErrorJson(ex);
                    status = statusForError(ex.what());
                }

                callback(makeJsonResponse(json, status));
            },
            {drogon::Post});
    }
} // namespace dorm_energy::web
