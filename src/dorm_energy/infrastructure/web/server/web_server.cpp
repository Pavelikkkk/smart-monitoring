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
            repository,
        std::shared_ptr<AuthService> authService)
        : aggregator_(std::move(aggregator)),
          repository_(std::move(repository)),
          authService_(std::move(authService))
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

        auto statusForError =
            [](const std::string &error)
            {
                if (error == "Missing token" ||
                    error == "User not found" ||
                    error.find("token") != std::string::npos ||
                    error.find("Invalid input") != std::string::npos ||
                    error.find("signature") != std::string::npos ||
                    error.find("issuer") != std::string::npos)
                {
                    return drogon::k401Unauthorized;
                }

                if (error == "Admin role is required")
                {
                    return drogon::k403Forbidden;
                }

                return drogon::k400BadRequest;
            };

        auto makeJsonResponse =
            [](const Json::Value &json,
               drogon::HttpStatusCode status = drogon::k200OK)
            {
                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(json);

                response->setStatusCode(status);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                return response;
            };

        drogon::app().registerHandler(
            "/api/auth/me",
            [this, statusForError, makeJsonResponse](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status =
                    drogon::k200OK;

                try
                {
                    auto auth =
                        req->getHeader(
                            "Authorization");

                    const std::string prefix =
                        "Bearer ";

                    if (auth.rfind(prefix, 0) != 0)
                    {
                        throw std::runtime_error(
                            "Missing token");
                    }

                    auto token =
                        auth.substr(
                            prefix.size());

                    auto claims =
                        authService_->validateToken(
                            token);

                    auto user =
                        repository_->getUserById(
                            claims.userId);

                    if (!user)
                    {
                        throw std::runtime_error(
                            "User not found");
                    }

                    json["id"] =
                        user->id;

                    json["username"] =
                        user->username;

                    json["email"] =
                        user->email;

                    json["role"] =
                        user->role;

                    json["organizationId"] =
                        user->organizationId;

                    json["accountType"] =
                        user->accountType;

                    json["telegramChatId"] =
                        user->telegramChatId;

                    json["success"] =
                        true;
                }
                catch (const std::exception &ex)
                {
                    json["success"] =
                        false;

                    json["error"] =
                        ex.what();

                    status =
                        statusForError(
                            ex.what());
                }

                callback(
                    makeJsonResponse(
                        json,
                        status));
            },
            {drogon::Get});

        auto requireUser =
            [this](
                const drogon::HttpRequestPtr &req)
            {
                auto auth =
                    req->getHeader(
                        "Authorization");

                const std::string prefix =
                    "Bearer ";

                if (auth.rfind(prefix, 0) != 0)
                {
                    throw std::runtime_error(
                        "Missing token");
                }

                auto claims =
                    authService_->validateToken(
                        auth.substr(
                            prefix.size()));

                auto user =
                    repository_->getUserById(
                        claims.userId);

                if (!user)
                {
                    throw std::runtime_error(
                        "User not found");
                }

                if (user->organizationId <= 0)
                {
                    throw std::runtime_error(
                        "User organization is not configured");
                }

                return *user;
            };

        auto requireAdmin =
            [requireUser](
                const drogon::HttpRequestPtr &req)
            {
                auto user =
                    requireUser(req);

                if (user.role != "ADMIN")
                {
                    throw std::runtime_error(
                        "Admin role is required");
                }

                return user;
            };

        drogon::app().registerHandler(
            "/api/admin/overview",
            [this, requireAdmin](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;

                try
                {
                    requireAdmin(req);
                    json = repository_->getAdminOverview();
                    json["success"] = true;
                }
                catch (const std::exception &ex)
                {
                    json["success"] = false;
                    json["error"] = ex.what();
                }

                auto response =
                    drogon::HttpResponse::
                        newHttpJsonResponse(json);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            },
            {drogon::Get});

        drogon::app().registerHandler(
            "/api/admin/buildings",
            [this, requireAdmin](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;

                try
                {
                    requireAdmin(req);

                    auto body =
                        req->getJsonObject();

                    if (!body)
                    {
                        throw std::runtime_error(
                            "Invalid JSON body");
                    }

                    auto id =
                        repository_->createBuildingForOrganization(
                            (*body)["organizationId"].asInt(),
                            (*body)["name"].asString(),
                            (*body)["address"].asString(),
                            (*body)["description"].asString());

                    json["success"] = true;
                    json["id"] = id;
                }
                catch (const std::exception &ex)
                {
                    json["success"] = false;
                    json["error"] = ex.what();
                }

                auto response =
                    drogon::HttpResponse::
                        newHttpJsonResponse(json);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            },
            {drogon::Post});

        drogon::app().registerHandler(
            "/api/admin/rooms",
            [this, requireAdmin](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;

                try
                {
                    requireAdmin(req);

                    auto body =
                        req->getJsonObject();

                    if (!body)
                    {
                        throw std::runtime_error(
                            "Invalid JSON body");
                    }

                    auto id =
                        repository_->createRoomForBuilding(
                            (*body)["buildingId"].asInt(),
                            (*body)["roomName"].asString(),
                            (*body)["roomType"].asString(),
                            (*body)["floorNumber"].asInt());

                    json["success"] = true;
                    json["id"] = id;
                }
                catch (const std::exception &ex)
                {
                    json["success"] = false;
                    json["error"] = ex.what();
                }

                auto response =
                    drogon::HttpResponse::
                        newHttpJsonResponse(json);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            },
            {drogon::Post});

        drogon::app().registerHandler(
            "/api/admin/devices",
            [this, requireAdmin](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;

                try
                {
                    requireAdmin(req);

                    auto body =
                        req->getJsonObject();

                    if (!body)
                    {
                        throw std::runtime_error(
                            "Invalid JSON body");
                    }

                    repository_->createDeviceForRoom(
                        (*body)["deviceId"].asString(),
                        (*body)["deviceName"].asString(),
                        (*body)["deviceModel"].asString(),
                        (*body)["firmwareVersion"].asString(),
                        (*body)["roomId"].asInt());

                    json["success"] = true;
                }
                catch (const std::exception &ex)
                {
                    json["success"] = false;
                    json["error"] = ex.what();
                }

                auto response =
                    drogon::HttpResponse::
                        newHttpJsonResponse(json);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            },
            {drogon::Post});
        // ============================================
        // /api/stats
        // ============================================

        drogon::app().registerHandler(
            "/api/stats",
            [this, requireUser, statusForError, makeJsonResponse](const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status =
                    drogon::k200OK;

                try
                {
                    auto user =
                        requireUser(req);

                    json["buildings"] =
                        static_cast<int>(
                            repository_->getBuildings(
                                user.organizationId)
                                .size());

                    json["rooms"] =
                        static_cast<int>(
                            repository_->getRooms(
                                user.organizationId)
                                .size());

                    json["devices"] =
                        static_cast<int>(
                            repository_->getDevices(
                                user.organizationId)
                                .size());

                    json["anomalies"] =
                        static_cast<int>(
                            repository_->getLatestAnomalies(
                                1000,
                                user.organizationId)
                                .size());

                    json["mqttOnline"] = true;
                    json["success"] = true;
                }
                catch (const std::exception &ex)
                {
                    json["success"] = false;
                    json["error"] = ex.what();
                    status =
                        statusForError(
                            ex.what());
                }

                callback(
                    makeJsonResponse(
                        json,
                        status));
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
            [this, requireUser, statusForError, makeJsonResponse](const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value anomalies(Json::arrayValue);

                try
                {
                    auto user =
                        requireUser(req);

                    auto latest =
                        repository_->getLatestAnomalies(
                            20,
                            user.organizationId);

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

                    callback(
                        makeJsonResponse(
                            anomalies));
                }
                catch (const std::exception &ex)
                {
                    Json::Value json;
                    json["success"] = false;
                    json["error"] = ex.what();
                    callback(
                        makeJsonResponse(
                            json,
                            statusForError(
                                ex.what())));
                }
            });

        // ============================================
        // /api/rooms
        // ============================================

        drogon::app().registerHandler(
            "/api/rooms",
            [this, requireUser, statusForError, makeJsonResponse](const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value rooms(Json::arrayValue);

                try
                {
                    auto user =
                        requireUser(req);

                    auto list =
                        repository_->getRooms(
                            user.organizationId);

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

                    callback(
                        makeJsonResponse(
                            rooms));
                }
                catch (const std::exception &ex)
                {
                    Json::Value json;
                    json["success"] = false;
                    json["error"] = ex.what();
                    callback(
                        makeJsonResponse(
                            json,
                            statusForError(
                                ex.what())));
                }
            });

        // ============================================
        // /api/devices
        // ============================================

        drogon::app().registerHandler(
            "/api/devices",
            [this, requireUser, statusForError, makeJsonResponse](const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value devices(Json::arrayValue);

                try
                {
                    auto user =
                        requireUser(req);

                    auto list =
                        repository_->getDevices(
                            user.organizationId);

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

                        item["roomId"] =
                            d.roomId;

                        item["buildingId"] =
                            d.buildingId;

                        item["organizationId"] =
                            d.organizationId;

                        item["isOnline"] =
                            d.isOnline;

                        item["lastSeenAt"] =
                            d.lastSeenAt;

                        devices.append(item);
                    }

                    callback(
                        makeJsonResponse(
                            devices));
                }
                catch (const std::exception &ex)
                {
                    Json::Value json;
                    json["success"] = false;
                    json["error"] = ex.what();
                    callback(
                        makeJsonResponse(
                            json,
                            statusForError(
                                ex.what())));
                }
            });
        drogon::app().registerHandler(
            "/api/analytics/top-consumers",
            [this, requireUser, statusForError, makeJsonResponse](const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value result(Json::arrayValue);

                try
                {
                    auto user =
                        requireUser(req);

                    auto consumers =
                        repository_->getTopConsumers(
                            10,
                            user.organizationId);

                    for (const auto &c : consumers)
                    {
                        Json::Value item;

                        item["roomName"] =
                            c.roomName;

                        item["power"] =
                            c.power;

                        result.append(item);
                    }

                    callback(
                        makeJsonResponse(
                            result));
                }
                catch (const std::exception &ex)
                {
                    Json::Value json;
                    json["success"] = false;
                    json["error"] = ex.what();
                    callback(
                        makeJsonResponse(
                            json,
                            statusForError(
                                ex.what())));
                }
            });

        drogon::app().registerHandler(
            "/api/analytics/anomalies-by-type",
            [this, requireUser, statusForError, makeJsonResponse](const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value result(Json::arrayValue);

                try
                {
                    auto user =
                        requireUser(req);

                    auto stats =
                        repository_->getAnomalyStatistics(
                            user.organizationId);

                    for (const auto &s : stats)
                    {
                        Json::Value item;

                        item["type"] =
                            s.type;

                        item["count"] =
                            s.count;

                        result.append(item);
                    }

                    callback(
                        makeJsonResponse(
                            result));
                }
                catch (const std::exception &ex)
                {
                    Json::Value json;
                    json["success"] = false;
                    json["error"] = ex.what();
                    callback(
                        makeJsonResponse(
                            json,
                            statusForError(
                                ex.what())));
                }
            });

        // ============================================
        // /api/buildings
        // ============================================

        drogon::app().registerHandler(
            "/api/buildings",
            [this, requireUser, statusForError, makeJsonResponse](const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value buildings(Json::arrayValue);

                try
                {
                    auto user =
                        requireUser(req);

                    auto list =
                        repository_->getBuildings(
                            user.organizationId);

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

                    callback(
                        makeJsonResponse(
                            buildings));
                }
                catch (const std::exception &ex)
                {
                    Json::Value json;
                    json["success"] = false;
                    json["error"] = ex.what();
                    callback(
                        makeJsonResponse(
                            json,
                            statusForError(
                                ex.what())));
                }
            });
        drogon::app().registerHandler(
            "/api/analytics/energy-by-room",
            [this, requireUser, statusForError, makeJsonResponse](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value data(
                    Json::arrayValue);

                try
                {
                    auto user =
                        requireUser(req);

                    auto rooms =
                        repository_->getEnergyByRoom(
                            user.organizationId);

                    for (const auto &room : rooms)
                    {
                        Json::Value item;

                        item["roomName"] =
                            room.roomName;

                        item["power"] =
                            room.avgPower;

                        data.append(item);
                    }

                    callback(
                        makeJsonResponse(
                            data));
                }
                catch (const std::exception &ex)
                {
                    Json::Value json;
                    json["success"] = false;
                    json["error"] = ex.what();
                    callback(
                        makeJsonResponse(
                            json,
                            statusForError(
                                ex.what())));
                }
            });
        drogon::app().registerHandler(
            "/api/analytics/severity-distribution",
            [this, requireUser, statusForError, makeJsonResponse](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value data(
                    Json::arrayValue);

                try
                {
                    auto user =
                        requireUser(req);

                    auto stats =
                        repository_->getSeverityDistribution(
                            user.organizationId);

                    for (const auto &s : stats)
                    {
                        Json::Value item;

                        item["severity"] =
                            s.severity;

                        item["count"] =
                            s.count;

                        data.append(item);
                    }

                    callback(
                        makeJsonResponse(
                            data));
                }
                catch (const std::exception &ex)
                {
                    Json::Value json;
                    json["success"] = false;
                    json["error"] = ex.what();
                    callback(
                        makeJsonResponse(
                            json,
                            statusForError(
                                ex.what())));
                }
            });

        // ============================================
        // /api/power/history
        // ============================================

        drogon::app().registerHandler(
            "/api/power/history",
            [this, requireUser, statusForError, makeJsonResponse](const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value history(Json::arrayValue);

                try
                {
                    auto user =
                        requireUser(req);

                    auto points =
                        repository_->getPowerHistory(
                            24,
                            user.organizationId);

                    for (const auto &p : points)
                    {
                        Json::Value point;

                        point["time"] =
                            p.time;

                        point["power"] =
                            p.power;

                        history.append(point);
                    }

                    callback(
                        makeJsonResponse(
                            history));
                }
                catch (const std::exception &ex)
                {
                    Json::Value json;
                    json["success"] = false;
                    json["error"] = ex.what();
                    callback(
                        makeJsonResponse(
                            json,
                            statusForError(
                                ex.what())));
                }
            });

        drogon::app().registerHandler(
            "/api/auth/register",
            [this](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value responseJson;

                try
                {
                    auto json =
                        req->getJsonObject();

                    if (!json)
                    {
                        throw std::runtime_error(
                            "Invalid JSON body");
                    }

                    auto accountType =
                        (*json)["accountType"].asString();

                    if (accountType.empty())
                    {
                        accountType = "PERSONAL";
                    }

                    auto userId =
                        authService_->registerUser(
                            (*json)["username"].asString(),
                            (*json)["email"].asString(),
                            (*json)["password"].asString(),
                            accountType);

                    responseJson["success"] = true;
                    responseJson["userId"] = userId;
                }
                catch (const std::exception &ex)
                {
                    responseJson["success"] = false;
                    responseJson["error"] = ex.what();
                }

                auto response =
                    drogon::HttpResponse::
                        newHttpJsonResponse(
                            responseJson);

                response->addHeader(
                    "Access-Control-Allow-Origin",
                    "*");

                callback(response);
            },
            {drogon::Post});

        drogon::app().registerHandler(
            "/api/auth/login",
            [this, makeJsonResponse](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value responseJson;
                auto status =
                    drogon::k200OK;

                try
                {
                    auto json =
                        req->getJsonObject();

                    if (!json)
                    {
                        throw std::runtime_error(
                            "Invalid JSON body");
                    }

                    auto token =
                        authService_->loginUser(
                            (*json)["email"].asString(),
                            (*json)["password"].asString());

                    responseJson["token"] =
                        token;

                    responseJson["success"] =
                        true;
                }
                catch (const std::exception &ex)
                {
                    responseJson["success"] =
                        false;

                    responseJson["error"] =
                        ex.what();

                    status =
                        drogon::k401Unauthorized;
                }

                callback(
                    makeJsonResponse(
                        responseJson,
                        status));
            },
            {drogon::Post});

        drogon::app().registerHandler(
            "/api/account",
            [this, statusForError, makeJsonResponse](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status =
                    drogon::k200OK;

                try
                {
                    auto auth = req->getHeader("Authorization");
                    const std::string prefix = "Bearer ";

                    if (auth.rfind(prefix, 0) != 0)
                    {
                        throw std::runtime_error("Missing token");
                    }

                    auto claims =
                        authService_->validateToken(
                            auth.substr(prefix.size()));

                    auto user =
                        repository_->getUserById(
                            claims.userId);

                    if (!user)
                    {
                        throw std::runtime_error("User not found");
                    }

                    auto subscription =
                        repository_->getUserSubscription(
                            claims.userId);

                    json["success"] = true;
                    json["id"] = user->id;
                    json["username"] = user->username;
                    json["email"] = user->email;
                    json["role"] = user->role;
                    json["organizationId"] = user->organizationId;
                    json["accountType"] = user->accountType;
                    json["telegramChatId"] = user->telegramChatId;
                    json["subscription"] = subscription;
                }
                catch (const std::exception &ex)
                {
                    json["success"] = false;
                    json["error"] = ex.what();
                    status =
                        statusForError(
                            ex.what());
                }

                callback(
                    makeJsonResponse(
                        json,
                        status));
            },
            {drogon::Get});

        drogon::app().registerHandler(
            "/api/account/telegram-chat-id",
            [this, statusForError, makeJsonResponse](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status =
                    drogon::k200OK;

                try
                {
                    auto auth = req->getHeader("Authorization");
                    const std::string prefix = "Bearer ";

                    if (auth.rfind(prefix, 0) != 0)
                    {
                        throw std::runtime_error("Missing token");
                    }

                    auto claims =
                        authService_->validateToken(
                            auth.substr(prefix.size()));

                    auto body =
                        req->getJsonObject();

                    if (!body)
                    {
                        throw std::runtime_error(
                            "Invalid JSON body");
                    }

                    auto updated =
                        repository_->updateUserTelegramChatId(
                            claims.userId,
                            (*body)["telegramChatId"].asString());

                    if (!updated)
                    {
                        throw std::runtime_error("User not found");
                    }

                    json["success"] = true;
                }
                catch (const std::exception &ex)
                {
                    json["success"] = false;
                    json["error"] = ex.what();
                    status =
                        statusForError(
                            ex.what());
                }

                callback(
                    makeJsonResponse(
                        json,
                        status));
            },
            {drogon::Put});

        drogon::app().registerHandler(
            "/api/subscription",
            [this, statusForError, makeJsonResponse](
                const drogon::HttpRequestPtr &req,
                std::function<void(
                    const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status =
                    drogon::k200OK;

                try
                {
                    auto auth = req->getHeader("Authorization");
                    const std::string prefix = "Bearer ";

                    if (auth.rfind(prefix, 0) != 0)
                    {
                        throw std::runtime_error("Missing token");
                    }

                    auto claims =
                        authService_->validateToken(
                            auth.substr(prefix.size()));

                    json =
                        repository_->getUserSubscription(
                            claims.userId);

                    json["success"] = true;
                }
                catch (const std::exception &ex)
                {
                    json["success"] = false;
                    json["error"] = ex.what();
                    status =
                        statusForError(
                            ex.what());
                }

                callback(
                    makeJsonResponse(
                        json,
                        status));
            },
            {drogon::Get});

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
