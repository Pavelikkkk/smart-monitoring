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
        <h1 className="text-3xl font-bold">
          Device not found
        </h1>

        <p className="text-slate-400 mt-2">
          Device ID: {id}
        </p>
      </div>
    );
  }

  return (
    <>
      <h1 className="text-4xl font-bold mb-2">
        {device.deviceName}
      </h1>

      <p className="text-slate-400 mb-8">
        Room: {device.roomName}
      </p>

      <div className="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-4 gap-4 mb-8">

        <div className="bg-slate-800 rounded-xl p-6">
          <p className="text-slate-400">
            Device ID
          </p>

          <h2 className="text-xl font-bold text-cyan-400">
            {device.deviceId}
          </h2>
        </div>

        <div className="bg-slate-800 rounded-xl p-6">
          <p className="text-slate-400">
            Model
          </p>

          <h2 className="text-xl font-bold">
            {device.deviceModel}
          </h2>
        </div>

        <div className="bg-slate-800 rounded-xl p-6">
          <p className="text-slate-400">
            Firmware
          </p>

          <h2 className="text-xl font-bold">
            {device.firmwareVersion}
          </h2>
        </div>

        <div className="bg-slate-800 rounded-xl p-6">
          <p className="text-slate-400">
            Status
          </p>

          <h2
            className={
              device.isOnline
                ? "text-green-400 font-bold"
                : "text-red-400 font-bold"
            }
          >
            {device.isOnline
              ? "ONLINE"
              : "OFFLINE"}
          </h2>
        </div>

      </div>

      <div className="bg-slate-800 border border-slate-700 rounded-xl p-6">

        <h2 className="text-2xl font-bold mb-6">
          Device Information
        </h2>

        <div className="space-y-4">

          <div className="flex justify-between">
            <span className="text-slate-400">
              Device Name
            </span>

            <span>
              {device.deviceName}
            </span>
          </div>

          <div className="flex justify-between">
            <span className="text-slate-400">
              Room
            </span>

            <span>
              {device.roomName}
            </span>
          </div>

          <div className="flex justify-between">
            <span className="text-slate-400">
              Model
            </span>

            <span>
              {device.deviceModel}
            </span>
          </div>

          <div className="flex justify-between">
            <span className="text-slate-400">
              Firmware
            </span>

            <span>
              {device.firmwareVersion}
            </span>
          </div>

          <div className="flex justify-between">
            <span className="text-slate-400">
              Last Seen
            </span>

            <span>
              {device.lastSeenAt || "N/A"}
            </span>
          </div>

        </div>

      </div>
    </>
  );
}