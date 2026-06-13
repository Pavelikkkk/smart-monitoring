import { request } from "../../shared/api/client";
import type { Device } from "../../shared/api/types";

export async function getUserDevices() {
  return request<Device[]>("/devices");
}

export async function getUserDevice(id: string) {
  const devices = await getUserDevices();

  return (
    devices.find((device) => String(device.deviceId) === String(id)) ?? null
  );
}
