#pragma once

#include <drogon/drogon.h>
#include <json/json.h>

#include <string>

namespace dorm_energy::web
{
    void addCorsHeaders(const drogon::HttpResponsePtr &response);

    drogon::HttpStatusCode statusForError(const std::string &error);

    drogon::HttpResponsePtr makeJsonResponse(const Json::Value &json,
                                             drogon::HttpStatusCode status = drogon::k200OK);

    Json::Value makeErrorJson(const std::exception &ex);
} // namespace dorm_energy::web
