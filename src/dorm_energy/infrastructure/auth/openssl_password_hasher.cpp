#include "dorm_energy/infrastructure/auth/openssl_password_hasher.hpp"

#include <openssl/sha.h>

#include <iomanip>
#include <sstream>

namespace
{
    constexpr const char *SALT = "dorm-energy-secret";
}

std::string OpenSslPasswordHasher::hash(
    const std::string &password)
{
    std::string input = password + SALT;
    unsigned char digest[SHA256_DIGEST_LENGTH];
    
    SHA256(reinterpret_cast<const unsigned char *>(input.c_str()), input.size(), digest);

    std::ostringstream stream;
    for (auto byte : digest)
    {
        stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }

    return stream.str();
}

bool OpenSslPasswordHasher::verify(
    const std::string &password,
    const std::string &hash)
{
    return this->hash(password) == hash;
}