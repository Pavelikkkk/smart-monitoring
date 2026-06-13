#pragma once

#include "dorm_energy/application/auth/auth_service.hpp"
#include "dorm_energy/domain/auth/user_claims.hpp"
#include "dorm_energy/domain/storage/imeasurement_repository.hpp"
#include "dorm_energy/domain/storage/user_dto.hpp"

#include <drogon/drogon.h>

#include <memory>

namespace dorm_energy::web
{
    class AuthMiddleware
    {
    public:
        AuthMiddleware(std::shared_ptr<dorm_energy::storage::IMeasurementRepository> repository,
                       std::shared_ptr<AuthService> authService);

        UserClaims requireClaims(const drogon::HttpRequestPtr &req) const;
        UserDto requireAuthenticatedUser(const drogon::HttpRequestPtr &req) const;
        UserDto requireUser(const drogon::HttpRequestPtr &req) const;
        UserDto requireAdmin(const drogon::HttpRequestPtr &req) const;

    private:
        std::shared_ptr<dorm_energy::storage::IMeasurementRepository> repository_;
        std::shared_ptr<AuthService> authService_;
    };
} // namespace dorm_energy::web
