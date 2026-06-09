import { useEffect, useState } from "react";
import { Link } from "react-router-dom";

import { getUserDevices } from "../services/api";

type Device = {
  deviceId: string;
  deviceName: string;
  deviceModel: string;
  firmwareVersion: string;
  roomName: string;
  isOnline: boolean;
  lastSeenAt: string;
};

export default function Devices() {
  const [devices, setDevices] =
    useState<Device[]>([]);

  const [search, setSearch] =
    useState("");

  const [statusFilter, setStatusFilter] =
    useState<
      "all" |
      "online" |
      "offline"
    >("all");

  useEffect(() => {
    loadDevices();
  }, []);

  async function loadDevices() {
    try {
      const data =
        await getUserDevices();

      setDevices(data);
    }
    catch (err) {
      console.error(err);
    }
  }

  const filteredDevices =
    devices.filter((device) => {

      const matchesSearch =
        device.deviceName
          .toLowerCase()
          .includes(
            search.toLowerCase()
          );

      const matchesStatus =
        statusFilter === "all"
        || (
          statusFilter === "online"
          && device.isOnline
        )
        || (
          statusFilter === "offline"
          && !device.isOnline
        );

      return (
        matchesSearch &&
        matchesStatus
      );
    });

  const onlineCount =
    devices.filter(
      (d) => d.isOnline
    ).length;

  const offlineCount =
    devices.filter(
      (d) => !d.isOnline
    ).length;

  return (
    <div className="space-y-8">

      {/* HEADER */}

      <div>

        <h1 className="text-5xl font-bold mb-3">
          Devices
        </h1>

        <p className="text-slate-300">
          Monitor and manage IoT devices.
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

        <div
          className="
            bg-[#111827]
            rounded-2xl
            p-5
          "
        >
          <p className="text-slate-300">
            📡 Devices
          </p>

          <h2 className="text-3xl font-bold">
            {devices.length}
          </h2>
        </div>

        <div
          className="
            bg-[#111827]
            rounded-2xl
            p-5
          "
        >
          <p className="text-slate-300">
            🟢 Online
          </p>

          <h2 className="text-3xl font-bold text-emerald-400">
            {onlineCount}
          </h2>
        </div>

        <div
          className="
            bg-[#111827]
            rounded-2xl
            p-5
          "
        >
          <p className="text-slate-300">
            🔴 Offline
          </p>

          <h2 className="text-3xl font-bold text-rose-400">
            {offlineCount}
          </h2>
        </div>

      </div>

      {/* SEARCH */}

      <input
        type="text"
        placeholder="Search devices..."
        value={search}
        onChange={(e) =>
          setSearch(
            e.target.value
          )
        }
        className="
          w-full
          bg-[#111827]
          border
          border-cyan-700/40
          rounded-2xl
          p-5
          outline-none
          focus:border-orange-300
        "
      />

      {/* FILTERS */}

      <div className="flex gap-3">

        <button
          onClick={() =>
            setStatusFilter("all")
          }
          className={`
            px-5
            py-2
            rounded-2xl
            ${statusFilter === "all"
            ? "text-cyan-400 text-slate-900"
            : "bg-[#111827]"
            }
          `}
        >
          All
        </button>

        <button
          onClick={() =>
            setStatusFilter("online")
          }
          className={`
            px-5
            py-2
            rounded-2xl
            ${statusFilter === "online"
              ? "bg-green-400 text-slate-900"
            : "bg-[#111827]"
            }
          `}
        >
          Online
        </button>

        <button
          onClick={() =>
            setStatusFilter("offline")
          }
          className={`
            px-5
            py-2
            rounded-2xl
            ${statusFilter === "offline"
              ? "bg-red-400 text-slate-900"
            : "bg-[#111827]"
            }
          `}
        >
          Offline
        </button>

      </div>

      {/* DEVICES */}

      <div
        className="
          grid
          grid-cols-1
          md:grid-cols-2
          xl:grid-cols-3
          gap-5
        "
      >

        {filteredDevices.map(
          (device) => (

            <Link
              key={device.deviceId}
              to={`/devices/${device.deviceId}`}
              className="
                bg-[#111827]
                border
                border-cyan-700/40
                rounded-2xl
                p-5
                hover:border-cyan-400
                hover:-translate-y-1
                transition
              "
            >

              <div className="flex justify-between mb-4">

                <h2
                  className="
                    text-xl
                    font-bold
                  "
                >
                  {device.deviceName}
                </h2>

                <span
                  className={
                    device.isOnline
                      ? "text-emerald-400"
                      : "text-rose-400"
                  }
                >
                  ●
                </span>

              </div>

              <p className="text-slate-300 mb-3">
                {device.roomName}
              </p>

              <div className="space-y-2">

                <div>
                  📡 {device.deviceModel}
                </div>

                <div>
                  ⚙️ {device.firmwareVersion}
                </div>

              </div>

              <div
                className="
                  mt-5
                  text-cyan-400
                  font-semibold
                "
              >
                View Device →
              </div>

            </Link>

          )
        )}

      </div>

    </div>
  );
}