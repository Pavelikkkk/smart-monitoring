import { request } from "../../shared/api/client";
import type { AdminOverview } from "../../shared/api/types";

export async function getAdminOverview() {
  return request<AdminOverview>("/admin/overview");
}

export async function createAdminBuilding(input: {
  organizationId: number;
  name: string;
  address: string;
  description: string;
}) {
  return request<{ id: number }>("/admin/buildings", {
    method: "POST",
    body: JSON.stringify(input),
  });
}

export async function createAdminRoom(input: {
  buildingId: number;
  roomName: string;
  roomType: string;
  floorNumber: number;
}) {
  return request<{ id: number }>("/admin/rooms", {
    method: "POST",
    body: JSON.stringify(input),
  });
}

export async function createAdminDevice(input: {
  deviceId: string;
  deviceName: string;
  deviceModel: string;
  firmwareVersion: string;
  roomId: number;
}) {
  return request<{ success: boolean }>("/admin/devices", {
    method: "POST",
    body: JSON.stringify(input),
  });
}
