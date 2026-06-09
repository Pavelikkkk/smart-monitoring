import { useEffect, useState } from "react";

import {
  getUserAnomalies,
  getUserRooms,
} from "../services/api";

export default function Anomalies() {
  const [anomalies, setAnomalies] =
    useState<any[]>([]);

  const [rooms, setRooms] =
    useState<any[]>([]);

  const [search, setSearch] =
    useState("");

  const [selectedRoom, setSelectedRoom] =
    useState("all");

  const [visibleCount, setVisibleCount] =
    useState(12);

  useEffect(() => {
    async function loadData() {
      try {
        const [
          anomaliesData,
          roomsData,
        ] = await Promise.all([
          getUserAnomalies(),
          getUserRooms(),
        ]);

        setAnomalies(anomaliesData);

        setRooms(roomsData);
      }
      catch (error) {
        console.error(error);
      }
    }

    loadData();
  }, []);

  function severityColor(
    severity: string
  ) {
    switch (severity) {
      case "CRITICAL":
        return "text-rose-400";

      case "WARNING":
        return "text-yellow-400";

      default:
        return "text-cyan-400";
    }
  }

  function borderColor(
    severity: string
  ) {
    switch (severity) {
      case "CRITICAL":
        return "border-t-rose-500";

      case "WARNING":
        return "border-t-yellow-500";

      default:
        return "border-t-cyan-500";
    }
  }

  const filteredAnomalies =
    anomalies.filter(
      (anomaly) => {
        const matchesSearch =
          `${anomaly.room} ${anomaly.type}`
            .toLowerCase()
            .includes(
              search.toLowerCase()
            );

        const matchesRoom =
          selectedRoom === "all" ||
          anomaly.room ===
          selectedRoom;

        return (
          matchesSearch &&
          matchesRoom
        );
      }
    );

  return (
    <div className="space-y-8">

      {/* HEADER */}

      <div>

        <h1 className="text-5xl font-bold mb-3">
          Anomalies
        </h1>

        <p className="text-slate-300">
          Browse detected anomalies
          across your rooms and devices.
        </p>

      </div>

      {/* FILTERS */}

      <div
        className="
          grid
          grid-cols-1
          md:grid-cols-2
          gap-4
        "
      >

        <input
          type="text"
          placeholder="Search anomalies..."
          value={search}
          onChange={(e) =>
            setSearch(
              e.target.value
            )
          }
          className="
            bg-[#111827]
            border
            border-cyan-700/40
            rounded-2xl
            p-5
            text-lg
            outline-none
            focus:border-cyan-400
          "
        />

        <select
          value={selectedRoom}
          onChange={(e) =>
            setSelectedRoom(
              e.target.value
            )
          }
          className="
            bg-[#111827]
            border
            border-cyan-700/40
            rounded-2xl
            p-5
            text-lg
            outline-none
            focus:border-cyan-400
          "
        >
          <option value="all">
            All Rooms
          </option>

          {rooms.map(
            (room) => (
              <option
                key={room.roomId}
                value={room.name}
              >
                {room.name}
              </option>
            )
          )}
        </select>

      </div>

      {/* ANOMALIES */}

      <div
        className="
          grid
          grid-cols-1
          md:grid-cols-2
          xl:grid-cols-3
          gap-5
        "
      >

        {filteredAnomalies
          .slice(
            0,
            visibleCount
          )
          .map(
            (
              anomaly,
              index
            ) => (
              <div
                key={`${anomaly.room}-${anomaly.type}-${index}`}
                className={`
                  bg-[#111827]
                  border
                  border-cyan-700/40
                  border-t-4
                  ${borderColor(
                    anomaly.severity
                  )}
                  rounded-2xl
                  p-5
                  hover:border-cyan-400
                  hover:-translate-y-1
                  transition
                `}
              >

                <div className="text-4xl mb-4">
                  ⚠️
                </div>

                <h2
                  className="
                    text-xl
                    font-bold
                    mb-4
                  "
                >
                  {anomaly.type}
                </h2>

                <div className="space-y-3">

                  <div>

                    <span className="text-slate-300">
                      Room:
                    </span>

                    {" "}
                    {anomaly.room}

                  </div>

                  <div>

                    <span className="text-slate-300">
                      Severity:
                    </span>

                    {" "}

                    <span
                      className={
                        severityColor(
                          anomaly.severity
                        )
                      }
                    >
                      {
                        anomaly.severity
                      }
                    </span>

                  </div>

                  <div>

                    <span className="text-slate-300">
                      Score:
                    </span>

                    {" "}

                    {Number(
                      anomaly.score
                    ).toFixed(3)}

                  </div>

                  <div
                    className="
                      text-slate-300
                      text-sm
                    "
                  >
                    {new Date(
                      anomaly.detectedAt
                    ).toLocaleString()}
                  </div>

                </div>

              </div>
            )
          )}

      </div>

      {/* SHOW MORE */}

      {visibleCount <
        filteredAnomalies.length && (
          <div className="flex justify-center">

          <button
            onClick={() =>
              setVisibleCount(
                visibleCount + 12
              )
            }
            className="
                px-8
                py-4
                rounded-2xl
                bg-cyan-500
                text-slate-900
                font-semibold
                hover:bg-cyan-400
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