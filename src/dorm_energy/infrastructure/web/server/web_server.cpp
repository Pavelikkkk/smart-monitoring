#include "dorm_energy/infrastructure/web/server/web_server.hpp"

#include <drogon/drogon.h>

namespace dorm_energy::web
{
    WebServer::WebServer(
        std::shared_ptr<
            dorm_energy::detection::RoomStateAggregator>
            aggregator,
        std::shared_ptr<
            dorm_energy::storage::IMeasurementRepository>
            repository)
        : aggregator_(std::move(aggregator)),
          repository_(std::move(repository))
    {
    }

    void WebServer::start()
    {
        drogon::app().addListener(
            "0.0.0.0",
            8080);

        // ============================================
        // CORS
        // ============================================

        drogon::app().registerPreRoutingAdvice(
            [](const drogon::HttpRequestPtr &req,
               drogon::AdviceCallback &&acb,
               drogon::AdviceChainCallback &&ccb)
            {
                auto response =
                    drogon::HttpResponse::newHttpResponse();

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                response->addHeader(
                    "Access-Control-Allow-Methods",
                    "GET, POST, PUT, DELETE, OPTIONS");

                response->addHeader(
                    "Access-Control-Allow-Headers",
                    "*");

                if (req->method() == drogon::Options)
                {
                    acb(response);
                    return;
                }

                ccb();
            });

        // ============================================
        // /api/stats
        // ============================================

        drogon::app().registerHandler(
            "/api/stats",
            [this](const drogon::HttpRequestPtr &,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;

                const auto &states =
                    aggregator_->getCurrentStates();

                json["rooms"] =
                    static_cast<int>(states.size());

                json["mqttOnline"] = true;

                int alerts = 0;
                int mlAlerts = 0;

                for (const auto &[id, state] : states)
                {
                    if (!state.motion &&
                        state.power > 150.0)
                    {
                        ++alerts;
                    }
                }

                json["alerts"] = alerts;
                json["mlAlerts"] = mlAlerts;

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(json);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });

        // ============================================
        // /api/health
        // ============================================

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

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(json);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });

        // ============================================
        // /api/anomalies/latest
        // ============================================

        drogon::app().registerHandler(
            "/api/anomalies/latest",
            [this](const drogon::HttpRequestPtr &,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value anomalies(Json::arrayValue);

                auto latest =
                    repository_->getLatestAnomalies(20);

                for (const auto &a : latest)
                {
                    Json::Value item;

                    item["room"] =
                        a.room;

                    item["type"] =
                        a.type;

                    item["severity"] =
                        a.severity;

                    item["score"] =
                        a.score;

                    item["description"] =
                        a.description;

                    item["detectedAt"] =
                        a.detectedAt;

                    anomalies.append(item);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        anomalies);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });

        // ============================================
        // /api/rooms
        // ============================================

        drogon::app().registerHandler(
            "/api/rooms",
            [this](const drogon::HttpRequestPtr &,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value rooms(Json::arrayValue);

                auto list =
                    repository_->getRooms();

                for (const auto &r : list)
                {
                    Json::Value room;

                    room["id"] =
                        r.id;

                    room["buildingId"] =
                        r.buildingId;

                    room["roomName"] =
                        r.roomName;

                    room["roomType"] =
                        r.roomType;

                    room["floorNumber"] =
                        r.floorNumber;

                    rooms.append(room);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        rooms);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });

        // ============================================
        // /api/devices
        // ============================================

        drogon::app().registerHandler(
            "/api/devices",
            [this](const drogon::HttpRequestPtr &,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value devices(Json::arrayValue);

                auto list =
                    repository_->getDevices();

                for (const auto &d : list)
                {
                    Json::Value item;

                    item["deviceId"] =
                        d.deviceId;

                    item["deviceName"] =
                        d.deviceName;

                    item["deviceModel"] =
                        d.deviceModel;

                    item["firmwareVersion"] =
                        d.firmwareVersion;

                    item["roomName"] =
                        d.roomName;

                    item["isOnline"] =
                        d.isOnline;

                    item["lastSeenAt"] =
                        d.lastSeenAt;

                    devices.append(item);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        devices);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });
        drogon::app().registerHandler(
            "/api/analytics/top-consumers",
            [this](const drogon::HttpRequestPtr &,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value result(Json::arrayValue);

                auto consumers =
                    repository_->getTopConsumers(10);

                for (const auto &c : consumers)
                {
                    Json::Value item;

                    item["roomName"] =
                        c.roomName;

                    item["power"] =
                        c.power;

                    result.append(item);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        result);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });

        drogon::app().registerHandler(
            "/api/analytics/anomalies-by-type",
            [this](const drogon::HttpRequestPtr &,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value result(Json::arrayValue);

                auto stats =
                    repository_->getAnomalyStatistics();

                for (const auto &s : stats)
                {
                    Json::Value item;

                    item["type"] =
                        s.type;

                    item["count"] =
                        s.count;

                    result.append(item);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        result);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });

        // ============================================
        // /api/buildings
        // ============================================

        drogon::app().registerHandler(
            "/api/buildings",
            [this](const drogon::HttpRequestPtr &,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value buildings(Json::arrayValue);

                auto list =
                    repository_->getBuildings();

                for (const auto &b : list)
                {
                    Json::Value item;

                    item["id"] =
                        b.id;

                    item["name"] =
                        b.name;

                    item["address"] =
                        b.address;

                    item["description"] =
                        b.description;

                    buildings.append(item);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        buildings);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });
        drogon::app().registerHandler(
            "/api/analytics/energy-by-room",
            [this](
                const drogon::HttpRequestPtr &,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value data(
                    Json::arrayValue);

                auto rooms =
                    repository_->getEnergyByRoom();

                for (const auto &room : rooms)
                {
                    Json::Value item;

                    item["roomName"] =
                        room.roomName;

                    item["power"] =
                        room.avgPower;

                    data.append(item);
                }

                auto response =
                    drogon::HttpResponse::
                        newHttpJsonResponse(data);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });
        drogon::app().registerHandler(
            "/api/analytics/severity-distribution",
            [this](
                const drogon::HttpRequestPtr &,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value data(
                    Json::arrayValue);

                auto stats =
                    repository_->getSeverityDistribution();

                for (const auto &s : stats)
                {
                    Json::Value item;

                    item["severity"] =
                        s.severity;

                    item["count"] =
                        s.count;

                    data.append(item);
                }

                auto response =
                    drogon::HttpResponse::
                        newHttpJsonResponse(data);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });

        // ============================================
        // /api/power/history
        // ============================================

        drogon::app().registerHandler(
            "/api/power/history",
            [this](const drogon::HttpRequestPtr &,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value history(Json::arrayValue);

                auto points =
                    repository_->getPowerHistory(24);

                for (const auto &p : points)
                {
                    Json::Value point;

                    point["time"] =
                        p.time;

                    point["power"] =
                        p.power;

                    history.append(point);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        history);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            });

        std::cout << "Starting Drogon..." << std::endl;

        server_thread_ = std::thread(
            []()
            {
                drogon::app().run();
            });
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