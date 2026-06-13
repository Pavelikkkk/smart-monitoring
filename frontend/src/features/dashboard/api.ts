import { getUserAnomalies } from "../anomalies/api";
import { getUserBuildings } from "../buildings/api";
import { getUserDevices } from "../devices/api";
import { getUserRooms } from "../rooms/api";
import type { Stats } from "../../shared/api/types";

export async function getUserStats() {
  const [buildings, rooms, devices, anomalies] = await Promise.all([
    getUserBuildings(),
    getUserRooms(),
    getUserDevices(),
    getUserAnomalies(),
  ]);

  return {
    buildings: buildings.length,
    rooms: rooms.length,
    devices: devices.length,
    anomalies: anomalies.length,
  } satisfies Stats;
}
