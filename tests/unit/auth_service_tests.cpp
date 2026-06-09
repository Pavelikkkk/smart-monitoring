#include "dorm_energy/application/auth/auth_service.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    class FakeRepository final : public dorm_energy::storage::IMeasurementRepository
    {
    public:
        bool save(const dorm_energy::core::SensorReading &) override { return true; }
        std::size_t saveBatch(const dorm_energy::core::ReadingsBatch &readings) override { return readings.size(); }
        void flush() override {}
        bool saveAnomaly(const dorm_energy::core::SensorReading &, const std::string &, dorm_energy::core::AlertSeverity, const std::string &, double) override { return true; }
        std::vector<dorm_energy::storage::AnomalyDto> getLatestAnomalies(std::size_t, int) override { return {}; }
        std::vector<dorm_energy::storage::PowerPointDto> getPowerHistory(int, int) override { return {}; }
        std::vector<dorm_energy::storage::DeviceDto> getDevices(int) override { return {}; }
        std::vector<dorm_energy::storage::BuildingDto> getBuildings(int) override { return {}; }
        std::vector<dorm_energy::storage::RoomDto> getRooms(int) override { return {}; }
        std::vector<dorm_energy::storage::TopConsumerDto> getTopConsumers(int, int) override { return {}; }
        std::vector<dorm_energy::storage::AnomalyStatsDto> getAnomalyStatistics(int) override { return {}; }
        std::vector<dorm_energy::storage::EnergyByRoomDto> getEnergyByRoom(int) override { return {}; }
        std::vector<dorm_energy::storage::SeverityStatsDto> getSeverityDistribution(int) override { return {}; }

        std::optional<UserDto> findUserByEmail(const std::string &email) override
        {
            if (user.has_value() && user->email == email)
            {
                return user;
            }
            return std::nullopt;
        }

        std::optional<UserDto> findUserById(int userId) override
        {
            if (user.has_value() && user->id == userId)
            {
                return user;
            }
            return std::nullopt;
        }

        int createUser(const UserDto &createdUser) override
        {
            lastCreated = createdUser;
            lastCreated->id = nextId;
            user = lastCreated;
            return nextId;
        }

        std::optional<UserDto> getUserById(int userId) override { return findUserById(userId); }
        bool updateUserTelegramChatId(int, const std::string &) override { return true; }
        Json::Value getUserSubscription(int) override { return Json::Value{}; }
        Json::Value getAdminOverview() override { return Json::Value{}; }
        int createBuildingForOrganization(int, const std::string &, const std::string &, const std::string &) override { return 1; }
        int createRoomForBuilding(int, const std::string &, const std::string &, int) override { return 1; }
        bool createDeviceForRoom(const std::string &, const std::string &, const std::string &, const std::string &, int) override { return true; }

        int nextId{42};
        std::optional<UserDto> user;
        std::optional<UserDto> lastCreated;
    };

    class FakePasswordHasher final : public IPasswordHasher
    {
    public:
        std::string hash(const std::string &password) override
        {
            lastPasswordToHash = password;
            return "hashed:" + password;
        }

        bool verify(const std::string &password, const std::string &hash) override
        {
            lastPasswordToVerify = password;
            lastHashToVerify = hash;
            return hash == "hashed:" + password;
        }

        std::string lastPasswordToHash;
        std::string lastPasswordToVerify;
        std::string lastHashToVerify;
    };

    class FakeJwtService final : public IJwtService
    {
    public:
        std::string generateToken(int userId, const std::string &email, const std::string &role) override
        {
            lastUserId = userId;
            lastEmail = email;
            lastRole = role;
            return "token:" + std::to_string(userId) + ":" + email + ":" + role;
        }

        UserClaims validateToken(const std::string &token) override
        {
            lastValidatedToken = token;
            return claims;
        }

        int extractUserId(const std::string &) override { return claims.userId; }

        int lastUserId{0};
        std::string lastEmail;
        std::string lastRole;
        std::string lastValidatedToken;
        UserClaims claims{7, "user@example.com", "USER"};
    };

    struct AuthFixture
    {
        std::shared_ptr<FakeRepository> repository{std::make_shared<FakeRepository>()};
        std::shared_ptr<FakePasswordHasher> hasher{std::make_shared<FakePasswordHasher>()};
        std::shared_ptr<FakeJwtService> jwt{std::make_shared<FakeJwtService>()};
        AuthService service{repository, hasher, jwt};
    };
}

TEST(AuthServiceTest, ValidatesRegistrationInput)
{
    AuthFixture fixture;

    EXPECT_THROW(fixture.service.registerUser("ab", "user@example.com", "password123", "PERSONAL"), std::runtime_error);
    EXPECT_THROW(fixture.service.registerUser("alice", "invalid", "password123", "PERSONAL"), std::runtime_error);
    EXPECT_THROW(fixture.service.registerUser("alice", "user@example.com", "short", "PERSONAL"), std::runtime_error);
}

TEST(AuthServiceTest, RejectsDuplicateRegistration)
{
    AuthFixture fixture;
    fixture.repository->user = UserDto{.id = 1, .username = "Alice", .email = "user@example.com", .passwordHash = "hash"};

    EXPECT_THROW(fixture.service.registerUser("alice", "user@example.com", "password123", "PERSONAL"), std::runtime_error);
}

TEST(AuthServiceTest, RegistersPersonalUserWithHashedPassword)
{
    AuthFixture fixture;

    const int id = fixture.service.registerUser("alice", "user@example.com", "password123", "SOMETHING_ELSE");

    ASSERT_TRUE(fixture.repository->lastCreated.has_value());
    EXPECT_EQ(id, 42);
    EXPECT_EQ(fixture.repository->lastCreated->username, "alice");
    EXPECT_EQ(fixture.repository->lastCreated->email, "user@example.com");
    EXPECT_EQ(fixture.repository->lastCreated->passwordHash, "hashed:password123");
    EXPECT_EQ(fixture.repository->lastCreated->role, "USER");
    EXPECT_EQ(fixture.repository->lastCreated->organizationId, 0);
    EXPECT_EQ(fixture.repository->lastCreated->accountType, "PERSONAL");
}

TEST(AuthServiceTest, RegistersBusinessAccountWhenRequested)
{
    AuthFixture fixture;

    fixture.service.registerUser("alice", "user@example.com", "password123", "BUSINESS");

    ASSERT_TRUE(fixture.repository->lastCreated.has_value());
    EXPECT_EQ(fixture.repository->lastCreated->accountType, "BUSINESS");
}

TEST(AuthServiceTest, LoginRejectsMissingUserAndWrongPassword)
{
    AuthFixture fixture;

    EXPECT_THROW(fixture.service.loginUser("missing@example.com", "password123"), std::runtime_error);

    fixture.repository->user = UserDto{
        .id = 9,
        .username = "Alice",
        .email = "user@example.com",
        .passwordHash = "hashed:correct-password",
        .role = "USER"};

    EXPECT_THROW(fixture.service.loginUser("user@example.com", "wrong-password"), std::runtime_error);
}

TEST(AuthServiceTest, LoginReturnsGeneratedToken)
{
    AuthFixture fixture;
    fixture.repository->user = UserDto{
        .id = 9,
        .username = "Alice",
        .email = "user@example.com",
        .passwordHash = "hashed:correct-password",
        .role = "ADMIN"};

    const auto token = fixture.service.loginUser("user@example.com", "correct-password");

    EXPECT_EQ(token, "token:9:user@example.com:ADMIN");
    EXPECT_EQ(fixture.jwt->lastUserId, 9);
    EXPECT_EQ(fixture.jwt->lastEmail, "user@example.com");
    EXPECT_EQ(fixture.jwt->lastRole, "ADMIN");
}

TEST(AuthServiceTest, ValidateTokenDelegatesToJwtService)
{
    AuthFixture fixture;
    fixture.jwt->claims = UserClaims{5, "admin@example.com", "ADMIN"};

    const auto claims = fixture.service.validateToken("jwt-token");

    EXPECT_EQ(fixture.jwt->lastValidatedToken, "jwt-token");
    EXPECT_EQ(claims.userId, 5);
    EXPECT_EQ(claims.email, "admin@example.com");
    EXPECT_EQ(claims.role, "ADMIN");
}
