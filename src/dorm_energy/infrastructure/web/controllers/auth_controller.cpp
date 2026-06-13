#include "dorm_energy/infrastructure/web/controllers/auth_controller.hpp"

#include "dorm_energy/infrastructure/web/middleware/auth_middleware.hpp"
#include "dorm_energy/infrastructure/web/utils/json_response.hpp"

#include <drogon/drogon.h>

namespace dorm_energy::web
{
    void registerAuthRoutes(const WebContext &context)
    {
        auto repository = context.repository;
        auto authService = context.authService;
        auto auth = AuthMiddleware(repository, authService);

        drogon::app().registerHandler(
            "/api/auth/me",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auto user = auth.requireAuthenticatedUser(req);

                    json["id"] = user.id;
                    json["username"] = user.username;
                    json["email"] = user.email;
                    json["role"] = user.role;
                    json["organizationId"] = user.organizationId;
                    json["accountType"] = user.accountType;
                    json["telegramChatId"] = user.telegramChatId;
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
            "/api/auth/register",
            [authService](const drogon::HttpRequestPtr &req,
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auto body = req->getJsonObject();

                    if (!body)
                    {
                        throw std::runtime_error("Invalid JSON body");
                    }

                    auto accountType = (*body)["accountType"].asString();

                    if (accountType.empty())
                    {
                        accountType = "PERSONAL";
                    }

                    auto userId = authService->registerUser(
                        (*body)["username"].asString(), (*body)["email"].asString(),
                        (*body)["password"].asString(), accountType);

                    json["success"] = true;
                    json["userId"] = userId;
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
            "/api/auth/login",
            [authService](const drogon::HttpRequestPtr &req,
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auto body = req->getJsonObject();

                    if (!body)
                    {
                        throw std::runtime_error("Invalid JSON body");
                    }

                    auto token = authService->loginUser((*body)["email"].asString(),
                                                        (*body)["password"].asString());

                    json["token"] = token;
                    json["success"] = true;
                }
                catch (const std::exception &ex)
                {
                    json = makeErrorJson(ex);
                    status = drogon::k401Unauthorized;
                }

                callback(makeJsonResponse(json, status));
            },
            {drogon::Post});

        drogon::app().registerHandler(
            "/api/account",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auto user = auth.requireAuthenticatedUser(req);
                    auto subscription = repository->getUserSubscription(user.id);

                    json["success"] = true;
                    json["id"] = user.id;
                    json["username"] = user.username;
                    json["email"] = user.email;
                    json["role"] = user.role;
                    json["organizationId"] = user.organizationId;
                    json["accountType"] = user.accountType;
                    json["telegramChatId"] = user.telegramChatId;
                    json["subscription"] = subscription;
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
            "/api/account/telegram-chat-id",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auto claims = auth.requireClaims(req);
                    auto body = req->getJsonObject();

                    if (!body)
                    {
                        throw std::runtime_error("Invalid JSON body");
                    }

                    auto updated = repository->updateUserTelegramChatId(
                        claims.userId, (*body)["telegramChatId"].asString());

                    if (!updated)
                    {
                        throw std::runtime_error("User not found");
                    }

                    json["success"] = true;
                }
                catch (const std::exception &ex)
                {
                    json = makeErrorJson(ex);
                    status = statusForError(ex.what());
                }

                callback(makeJsonResponse(json, status));
            },
            {drogon::Put});

        drogon::app().registerHandler(
            "/api/subscription",
            [repository, auth](const drogon::HttpRequestPtr &req,
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback)
            {
                Json::Value json;
                auto status = drogon::k200OK;

                try
                {
                    auto claims = auth.requireClaims(req);

                    json = repository->getUserSubscription(claims.userId);
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
    }
} // namespace dorm_energy::web
