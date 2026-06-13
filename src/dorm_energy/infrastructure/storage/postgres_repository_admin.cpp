#include "dorm_energy/infrastructure/storage/postgres_repository.hpp"

namespace dorm_energy::storage
{

    Json::Value PostgresMeasurementRepository::getAdminOverview()
    {
        pqxx::work txn(*connection_);

        Json::Value json;
        json["users"] = Json::Value(Json::arrayValue);
        json["buildings"] = Json::Value(Json::arrayValue);
        json["rooms"] = Json::Value(Json::arrayValue);

        auto users = txn.exec(
            R"(
            SELECT
                u.id,
                u.username,
                u.email,
                u.role,
                COALESCE(u.account_type, 'PERSONAL') AS account_type,
                COALESCE(u.organization_id, 0) AS organization_id,
                COALESCE(o.name, '') AS organization_name,
                COALESCE(s.plan, 'STANDARD') AS plan,
                COUNT(DISTINCT b.id) AS buildings_count,
                COUNT(DISTINCT r.id) AS rooms_count,
                COUNT(DISTINCT d.device_id) AS devices_count
            FROM users u
            LEFT JOIN organizations o
                ON o.id = u.organization_id
            LEFT JOIN subscriptions s
                ON s.user_id = u.id
            LEFT JOIN buildings b
                ON b.organization_id = u.organization_id
            LEFT JOIN rooms r
                ON r.building_id = b.id
            LEFT JOIN devices d
                ON d.room_id = r.id
            GROUP BY
                u.id,
                u.username,
                u.email,
                u.role,
                u.account_type,
                u.organization_id,
                o.name,
                s.plan
            ORDER BY u.id
            )");

        for (const auto &row : users)
        {
            Json::Value item;
            item["id"] = row["id"].as<int>();
            item["username"] = row["username"].c_str();
            item["email"] = row["email"].c_str();
            item["role"] = row["role"].c_str();
            item["accountType"] = row["account_type"].c_str();
            item["organizationId"] = row["organization_id"].as<int>(0);
            item["organizationName"] = row["organization_name"].c_str();
            item["plan"] = row["plan"].c_str();
            item["buildingsCount"] = row["buildings_count"].as<int>(0);
            item["roomsCount"] = row["rooms_count"].as<int>(0);
            item["devicesCount"] = row["devices_count"].as<int>(0);

            json["users"].append(item);
        }

        auto buildings = txn.exec(
            R"(
            SELECT
                id,
                organization_id,
                name,
                COALESCE(address, '') AS address
            FROM buildings
            ORDER BY organization_id, name
            )");

        for (const auto &row : buildings)
        {
            Json::Value item;
            item["id"] = row["id"].as<int>();
            item["organizationId"] = row["organization_id"].as<int>(0);
            item["name"] = row["name"].c_str();
            item["address"] = row["address"].c_str();

            json["buildings"].append(item);
        }

        auto rooms = txn.exec(
            R"(
            SELECT
                id,
                building_id,
                room_name,
                room_type,
                floor_number
            FROM rooms
            ORDER BY building_id, room_name
            )");

        for (const auto &row : rooms)
        {
            Json::Value item;
            item["id"] = row["id"].as<int>();
            item["buildingId"] = row["building_id"].as<int>();
            item["roomName"] = row["room_name"].c_str();
            item["roomType"] = row["room_type"].c_str();
            item["floorNumber"] = row["floor_number"].as<int>();

            json["rooms"].append(item);
        }

        return json;
    }

} // namespace dorm_energy::storage
