#include "dorm_energy/infrastructure/auth/jwt_service.hpp"

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

#include <chrono>
#include <stdexcept>

JwtService::JwtService(
    const std::string &secret)
    : secret_(secret)
{
}

std::string
JwtService::generateToken(
    int userId,
    const std::string &email,
    const std::string &role)
{
    using traits =
        jwt::traits::nlohmann_json;

    auto token =
        jwt::create<traits>()
            .set_type("JWT")
            .set_issuer("dorm-energy")
            .set_payload_claim(
                "user_id",
                jwt::basic_claim<traits>(
                    std::to_string(userId)))
            .set_payload_claim(
                "email",
                jwt::basic_claim<traits>(
                    email))
            .set_payload_claim(
                "role",
                jwt::basic_claim<traits>(
                    role))
            .set_expires_at(
                std::chrono::system_clock::now() + std::chrono::hours(24))
            .sign(
                jwt::algorithm::hs256{
                    secret_});

    return token;
}

UserClaims
JwtService::validateToken(
    const std::string &token)
{
    using traits =
        jwt::traits::nlohmann_json;

    auto decoded =
        jwt::decode<traits>(
            token);

    auto verifier =
        jwt::verify<traits>()
            .allow_algorithm(
                jwt::algorithm::hs256{
                    secret_})
            .with_issuer(
                "dorm-energy");

    verifier.verify(
        decoded);

    UserClaims claims;

    claims.userId =
        std::stoi(
            decoded
                .get_payload_claim(
                    "user_id")
                .as_string());

    claims.email =
        decoded
            .get_payload_claim(
                "email")
            .as_string();

    claims.role =
        decoded
            .get_payload_claim(
                "role")
            .as_string();

    return claims;
}
int
    JwtService::extractUserId(
        const std::string &token)
    {
        auto claims =
            validateToken(
                token);

        return claims.userId;
    }