import { useEffect, useState } from "react";
import { Link } from "react-router-dom";

import { getUserRooms, getUserBuildings } from "../services/api";
import type { Building, Room } from "../services/api";

export default function Rooms() {
  const [rooms, setRooms] = useState<Room[]>([]);

  const [buildings, setBuildings] = useState<Building[]>([]);

  const [search, setSearch] = useState("");

  const [selectedBuilding, setSelectedBuilding] = useState("all");

  useEffect(() => {
    async function loadData() {
      try {
        const [roomsData, buildingsData] = await Promise.all([
          getUserRooms(),
          getUserBuildings(),
        ]);

        setRooms(roomsData);
        setBuildings(buildingsData);
      } catch (error) {
        console.error(error);
      }
    }

    loadData();
  }, []);

  const filteredRooms = rooms.filter((room) => {
    const matchesSearch = room.name
      .toLowerCase()
      .includes(search.toLowerCase());

    const matchesBuilding =
      selectedBuilding === "all" ||
      room.buildingId.toString() === selectedBuilding;

    return matchesSearch && matchesBuilding;
  });

  return (
    <div className="space-y-8">
      {/* HEADER */}

      <div>
        <h1 className="text-5xl
              font-bold
              mb-2">Rooms</h1>

        <p className="text-slate-300
              text-lg">
          Browse and monitor connected rooms.
        </p>
      </div>

      {/* FILTERS */}

      <div
        className="grid
              grid-cols-1
              md:grid-cols-2
              gap-4"
      >
        <input
          type="text"
          placeholder="Search rooms..."
          value={search}
          onChange={(e) => setSearch(e.target.value)}
          className="bg-[#111827]
                border
                border-cyan-700/40
                rounded-2xl
                p-5
                text-lg
                outline-none
                focus:border-cyan-400"
        />

        <select
          value={selectedBuilding}
          onChange={(e) => setSelectedBuilding(e.target.value)}
          className="bg-[#111827]
                border
                border-cyan-700/40
                rounded-2xl
                p-5
                text-lg
                outline-none
                focus:border-cyan-400"
        >
          <option value="all">All Buildings</option>

          {buildings.map((building) => (
            <option key={building.id} value={building.id}>
              {building.name}
            </option>
          ))}
        </select>
      </div>

      {/* ROOMS */}

      <div
        className="grid
              grid-cols-1
              md:grid-cols-2
              xl:grid-cols-3
              gap-5"
      >
        {filteredRooms.map((room) => {
          const building = buildings.find((b) => b.id === room.buildingId);

          return (
            <Link
              key={room.roomId}
              to={`/rooms/${room.roomId}`}
              className="bg-[#111827]
                    border
                    border-cyan-700/40
                    rounded-2xl
                    p-5
                    hover:border-cyan-400
                    hover:-translate-y-1
                    transition"
            >
              <div className="text-4xl
                    mb-4">🚪</div>

              <h2
                className="text-2xl
                      font-bold
                      text-cyan-400
                      mb-3"
              >
                {room.name}
              </h2>

              <div className="space-y-2">
                <p className="text-slate-300">Building:</p>

                <p>{building?.name ?? "Unknown"}</p>

                <p className="text-slate-300
                      mt-3">Devices:</p>

                <p>{room.deviceCount}</p>
              </div>

              <div
                className="mt-6
                      text-cyan-400
                      font-semibold"
              >
                Open Room →
              </div>
            </Link>
          );
        })}
      </div>
    </div>
  );
}
