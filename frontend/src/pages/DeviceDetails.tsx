import { useEffect, useState } from "react";
import { useParams } from "react-router-dom";

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

export default function DeviceDetails() {
  const { id } = useParams();

  const [device, setDevice] =
    useState<Device | null>(null);

  const [loading, setLoading] =
    useState(true);

  useEffect(() => {
    if (id)
    {
      loadDevice();
    }
  }, [id]);

  async function loadDevice() {
    try {
      const devices =
        await getDevices();

      const current =
        devices.find(
          (d: Device) =>
            String(d.deviceId) === String(id)
        );

      setDevice(current || null);
    }
    catch (error) {
      console.error(error);
    }
    finally {
      setLoading(false);
    }
  }

  if (loading)
  {
    return (
      <div>
        Loading...
      </div>
    );
  }

  if (!device)
  {
    return (
      <div>
        <h1 className="text-4xl font-bold">
          Device not found
        </h1>
      </div>
    );
  }

  return (
    <div className="space-y-8">

      {/* HEADER */}

      <div>

        <div className="flex items-center gap-3 mb-3">

          <span
            className={
              device.isOnline
                ? "text-emerald-400 text-2xl"
                : "text-rose-400 text-2xl"
            }
          >
            ●
          </span>

          <h1 className="text-5xl font-bold">
            {device.deviceName}
          </h1>

        </div>

        <p className="text-slate-500 text-lg">
          {device.roomName}
        </p>

      </div>

      {/* STATS */}

      <div
        className="
          grid
          grid-cols-1
          md:grid-cols-2
          xl:grid-cols-4
          gap-4
        "
      >

        <div
          className="
            bg-[#111827]
            rounded-2xl
            p-6
          "
        >
          <p className="text-slate-500">
            Device ID
          </p>

          <h2
            className="
              text-xl
              font-bold
              text-cyan-400
            "
          >
            {device.deviceId}
          </h2>
        </div>

        <div
          className="
            bg-[#111827]
            rounded-2xl
            p-6
          "
        >
          <p className="text-slate-500">
            Model
          </p>

          <h2 className="text-xl font-bold">
            {device.deviceModel}
          </h2>
        </div>

        <div
          className="
            bg-[#111827]
            rounded-2xl
            p-6
          "
        >
          <p className="text-slate-500">
            Firmware
          </p>

          <h2 className="text-xl font-bold">
            {device.firmwareVersion}
          </h2>
        </div>

        <div
          className="
            bg-[#111827]
            rounded-2xl
            p-6
          "
        >
          <p className="text-slate-500">
            Status
          </p>

          <h2
            className={
              device.isOnline
                ? "text-xl font-bold text-emerald-400"
                : "text-xl font-bold text-rose-400"
            }
          >
            {
              device.isOnline
                ? "ONLINE"
                : "OFFLINE"
            }
          </h2>
        </div>

      </div>

      {/* DEVICE INFORMATION */}

      <div
        className="
          bg-[#111827]
          border
          border-cyan-900/40
          rounded-2xl
          p-6
        "
      >

        <h2 className="text-2xl font-bold mb-6">
          Device Information
        </h2>

        <div className="space-y-5">

          <div className="flex justify-between">

            <span className="text-slate-500">
              Device Name
            </span>

            <span>
              {device.deviceName}
            </span>

          </div>

          <div className="flex justify-between">

            <span className="text-slate-500">
              Room
            </span>

            <span>
              {device.roomName}
            </span>

          </div>

          <div className="flex justify-between">

            <span className="text-slate-500">
              Device Model
            </span>

            <span>
              {device.deviceModel}
            </span>

          </div>

          <div className="flex justify-between">

            <span className="text-slate-500">
              Firmware
            </span>

            <span>
              {device.firmwareVersion}
            </span>

          </div>

          <div className="flex justify-between">

            <span className="text-slate-500">
              Last Seen
            </span>

            <span>
              {
                device.lastSeenAt ||
                "N/A"
              }
            </span>

          </div>

        </div>

      </div>

    </div>
  );
}