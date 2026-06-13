#pragma once

#include "dorm_energy/domain/auth/user_claims.hpp"

#include <string>

class IJwtService
{
public:
    virtual ~IJwtService() = default;

    virtual std::string generateToken(int userId, const std::string &email,
                                      const std::string &role) = 0;

    virtual UserClaims validateToken(const std::string &token) = 0;

    virtual int extractUserId(const std::string &token) = 0;
};