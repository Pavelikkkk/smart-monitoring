import { useEffect, useState } from "react";
import { Link } from "react-router-dom";

import { getDevices } from "../services/api";

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

  useEffect(() => {
    loadDevices();
  }, []);

  async function loadDevices() {
    try {
      const data =
        await getDevices();

      setDevices(data);
    }
    catch (err) {
      console.error(err);
    }
  }

  return (
    <>
      <h1 className="text-4xl font-bold mb-8">
        Devices
      </h1>

      <div className="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-3 gap-6">

        {devices.map((device) => (
          <Link
            key={device.deviceId}
            to={`/devices/${device.deviceId}`}
            className="
              block
              bg-slate-800
              border
              border-slate-700
              rounded-xl
              p-6
              hover:border-cyan-500
              transition
            "
          >
            <div className="flex justify-between mb-4">

              <h2 className="text-xl font-bold text-cyan-400">
                {device.deviceName}
              </h2>

              <span
                className={
                  device.isOnline
                    ? "text-green-400"
                    : "text-red-400"
                }
              >
                ●
              </span>

            </div>

            <div className="space-y-2">

              <div>
                <span className="text-slate-400">
                  Room:
                </span>{" "}
                {device.roomName}
              </div>

              <div>
                <span className="text-slate-400">
                  Model:
                </span>{" "}
                {device.deviceModel}
              </div>

              <div>
                <span className="text-slate-400">
                  Firmware:
                </span>{" "}
                {device.firmwareVersion}
              </div>

            </div>

            <div className="mt-4 text-cyan-400 text-sm font-semibold">
              View Device →
            </div>

          </Link>
        ))}

      </div>
    </>
  );
}