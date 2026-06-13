#include "dorm_energy/infrastructure/web/controllers/resources_controller.hpp"

#include "dorm_energy/infrastructure/web/middleware/auth_middleware.hpp"
#include "dorm_energy/infrastructure/web/utils/json_response.hpp"

#include <drogon/drogon.h>

namespace dorm_energy::web
{
    void registerResourceRoutes(const WebContext &context)
    {
        auto repository = context.repository;
        auto auth = AuthMiddleware(repository, context.authService);

        drogon::app().registerHandler(
            "/api/stats",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auto user = auth.requireUser(req);

                    json["buildings"] =
                        static_cast<int>(repository->getBuildings(user.organizationId).size());
                    json["rooms"] =
                        static_cast<int>(repository->getRooms(user.organizationId).size());
                    json["devices"] =
                        static_cast<int>(repository->getDevices(user.organizationId).size());
                    json["anomalies"] = static_cast<int>(
                        repository->getLatestAnomalies(1000, user.organizationId).size());
                    json["mqttOnline"] = true;
                    json["success"] = true;
                }
                catch (const std::exception &ex)
                {
                    json = makeErrorJson(ex);
                    status = statusForError(ex.what());
                }

                callback(makeJsonResponse(json, status));
            });

        drogon::app().registerHandler(
            "/api/buildings",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value buildings(Json::arrayValue);

                try
                {
                    auto user = auth.requireUser(req);
                    auto list = repository->getBuildings(user.organizationId);

                    for (const auto &building : list)
                    {
                        Json::Value item;

                        item["id"] = building.id;
                        item["name"] = building.name;
                        item["address"] = building.address;
                        item["description"] = building.description;

                        buildings.append(item);
                    }

                    callback(makeJsonResponse(buildings));
                }
                catch (const std::exception &ex)
                {
                    callback(makeJsonResponse(makeErrorJson(ex), statusForError(ex.what())));
                }
            });

        drogon::app().registerHandler(
            "/api/rooms",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value rooms(Json::arrayValue);

                try
                {
                    auto user = auth.requireUser(req);
                    auto list = repository->getRooms(user.organizationId);

                    for (const auto &room : list)
                    {
                        Json::Value item;

                        item["id"] = room.id;
                        item["buildingId"] = room.buildingId;
                        item["roomName"] = room.roomName;
                        item["roomType"] = room.roomType;
                        item["floorNumber"] = room.floorNumber;

                        rooms.append(item);
                    }

                    callback(makeJsonResponse(rooms));
                }
                catch (const std::exception &ex)
                {
                    callback(makeJsonResponse(makeErrorJson(ex), statusForError(ex.what())));
                }
            });

        drogon::app().registerHandler(
            "/api/devices",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value devices(Json::arrayValue);

                try
                {
                    auto user = auth.requireUser(req);
                    auto list = repository->getDevices(user.organizationId);

                    for (const auto &device : list)
                    {
                        Json::Value item;

                        item["deviceId"] = device.deviceId;
                        item["deviceName"] = device.deviceName;
                        item["deviceModel"] = device.deviceModel;
                        item["firmwareVersion"] = device.firmwareVersion;
                        item["roomName"] = device.roomName;
                        item["roomId"] = device.roomId;
                        item["buildingId"] = device.buildingId;
                        item["organizationId"] = device.organizationId;
                        item["isOnline"] = device.isOnline;
                        item["lastSeenAt"] = device.lastSeenAt;

                        devices.append(item);
                    }

                    callback(makeJsonResponse(devices));
                }
                catch (const std::exception &ex)
                {
                    callback(makeJsonResponse(makeErrorJson(ex), statusForError(ex.what())));
                }
            });
    }
} // namespace dorm_energy::web
