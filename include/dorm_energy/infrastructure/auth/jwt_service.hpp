#pragma once

#include "dorm_energy/domain/auth/ijwt_service.hpp"

class JwtService : public IJwtService
{
public:
    explicit JwtService(const std::string &secret);

    std::string generateToken(int userId, const std::string &email,
                              const std::string &role) override;

    UserClaims validateToken(const std::string &token) override;

    int extractUserId(const std::string &token) override;

private:
    std::string secret_;
};