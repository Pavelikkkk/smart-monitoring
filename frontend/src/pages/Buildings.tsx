import { useEffect, useState } from "react";
import { Link } from "react-router-dom";

import {
  getBuildings,
  getRooms,
  getDevices,
} from "../services/api";

export default function Buildings() {
  const [buildings, setBuildings] =
    useState<any[]>([]);

  const [rooms, setRooms] =
    useState<any[]>([]);

  const [devices, setDevices] =
    useState<any[]>([]);

  const [search, setSearch] =
    useState("");

  const [visibleCount, setVisibleCount] =
    useState(5);

  useEffect(() => {
    async function loadData() {
      try {
        const [
          buildingsData,
          roomsData,
          devicesData,
        ] = await Promise.all([
          getBuildings(),
          getRooms(),
          getDevices(),
        ]);

        setBuildings(buildingsData);
        setRooms(roomsData);
        setDevices(devicesData);
      }
      catch (error) {
        console.error(error);
      }
    }

    loadData();
  }, []);

  const filteredBuildings =
    buildings.filter((building) =>
      building.name
        .toLowerCase()
        .includes(search.toLowerCase())
    );

  return (
    <div className="space-y-8">

      {/* HEADER */}

      <div>

        <h1 className="text-5xl font-bold mb-2">
          Buildings
        </h1>

        <p className="text-slate-500 text-lg">
          Manage monitored buildings and
          energy analytics.
        </p>

      </div>

      {/* STATS */}

      <div
        className="
          grid
          grid-cols-1
          md:grid-cols-3
          gap-4
        "
      >

        <div className="bg-[#111827] border border-cyan-900/40 rounded-2xl p-5">
          <p className="text-slate-500">
            🏢 Buildings
          </p>

          <h2 className="text-3xl font-bold">
            {buildings.length}
          </h2>
        </div>

        <div className="bg-[#111827] border border-cyan-900/40 rounded-2xl p-5">
          <p className="text-slate-500">
            🚪 Rooms
          </p>

          <h2 className="text-3xl font-bold">
            {rooms.length}
          </h2>
        </div>

        <div className="bg-[#111827] border border-cyan-900/40 rounded-2xl p-5">
          <p className="text-slate-500">
            📡 Devices
          </p>

          <h2 className="text-3xl font-bold">
            {devices.length}
          </h2>
        </div>

      </div>

      {/* SEARCH */}

      <input
        type="text"
        placeholder="Search buildings..."
        value={search}
        onChange={(e) =>
          setSearch(e.target.value)
        }
        className="
          w-full
          bg-[#111827]
          border
          border-cyan-900/40
          rounded-2xl
          p-5
          text-lg
          outline-none
          focus:border-orange-300
        "
      />

      {/* BUILDINGS */}

      <div
        className="
          grid
          grid-cols-1
          md:grid-cols-2
          xl:grid-cols-3
          gap-6
        "
      >

        {filteredBuildings
          .slice(0, visibleCount)
          .map((building) => (

            <Link
              key={building.id}
              to={`/buildings/${building.id}`}
              className="
                bg-[#111827]
                border
                border-cyan-900/40
                rounded-2xl
                p-6
                hover:border-cyan-400
                hover:-translate-y-1
                transition
              "
            >
              <div className="text-4xl mb-4">
                🏢
              </div>

              <h2
                className="
                  text-2xl
                  font-bold
                  text-cyan-400
                  mb-2
                "
              >
                {building.name}
              </h2>

              <p className="text-slate-500 mb-4">
                {building.address}
              </p>

              <p className="text-slate-300 mb-6">
                {
                  building.description
                }
              </p>

              <div
                className="
                  text-cyan-400
                  font-semibold
                "
              >
                View Building →
              </div>

            </Link>

          ))}

      </div>

      {/* SHOW MORE */}

      {visibleCount <
        filteredBuildings.length && (
          <div className="flex justify-center">

            <button
              onClick={() =>
                setVisibleCount(
                  visibleCount + 5
                )
              }
              className="
              px-8
              py-4
              rounded-2xl
              bg-orange-400
              text-slate-900
              font-semibold
              hover:text-cyan-400
              transition
            "
            >
              Show More
            </button>

          </div>
        )}

    </div>
  );
}