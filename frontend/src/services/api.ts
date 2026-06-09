const API_BASE_URL =
  import.meta.env.VITE_API_URL ?? "http://localhost:8080/api";
  import.meta.env.API_UEL ?? "https://aquarium-thinks-thursday-criterion.trycloudflare.com"
const TOKEN_KEY = "dorm_energy_token";

type ApiResult<T> = T & {
  success?: boolean;
  error?: string;
};

export type AccountType = "PERSONAL" | "BUSINESS";

export type Subscription = {
  plan: string;
  status: string;
  maxBuildings: number;
  maxRooms: number;
  maxDevices: number;
};

export type Account = {
  id: number;
  username: string;
  email: string;
  role: string;
  organizationId: number;
  accountType: AccountType;
  telegramChatId: string;
  subscription: Subscription;
};

type Stats = {
  buildings: number;
  rooms: number;
  devices: number;
  anomalies: number;
};

type Building = {
  id: number;
  name: string;
  address: string;
  description: string;
};

type Room = {
  id: number;
  roomId: number;
  buildingId: number;
  name: string;
  roomName: string;
  buildingName: string;
  motion: boolean;
  power: number;
  light: number;
};

type Device = {
  deviceId: string;
  deviceName: string;
  deviceModel: string;
  firmwareVersion: string;
  roomId: number;
  buildingId: number;
  organizationId: number;
  roomName: string;
  isOnline: boolean;
  lastSeenAt: string;
};

type Anomaly = {
  room: string;
  type: string;
  severity: string;
  score: number;
  status?: string;
};

export type AdminUser = {
  id: number;
  username: string;
  email: string;
  role: string;
  accountType: AccountType;
  organizationId: number;
  organizationName: string;
  plan: string;
  buildingsCount: number;
  roomsCount: number;
  devicesCount: number;
};

export type AdminBuilding = {
  id: number;
  organizationId: number;
  name: string;
  address: string;
};

export type AdminRoom = {
  id: number;
  buildingId: number;
  roomName: string;
  roomType: string;
  floorNumber: number;
};

export type AdminOverview = {
  users: AdminUser[];
  buildings: AdminBuilding[];
  rooms: AdminRoom[];
};

function getToken() {
  return localStorage.getItem(TOKEN_KEY);
}

export function setToken(token: string) {
  localStorage.setItem(TOKEN_KEY, token);
}

export function clearToken() {
  localStorage.removeItem(TOKEN_KEY);
}

async function request<T>(
  path: string,
  options: RequestInit = {}
): Promise<T> {
  const headers =
    new Headers(options.headers);

  headers.set("Content-Type", "application/json");

  const token = getToken();

  if (token) {
    headers.set("Authorization", `Bearer ${token}`);
  }

  const response =
    await fetch(`${API_BASE_URL}${path}`, {
      ...options,
      headers,
    });

  const data =
    (await response.json()) as ApiResult<T>;

  if (!response.ok || data.success === false) {
    throw new Error(data.error ?? "Request failed");
  }

  return data as T;
}

export async function registerUser(input: {
  username: string;
  email: string;
  password: string;
  accountType: AccountType;
}) {
  return request<{ userId: number }>("/auth/register", {
    method: "POST",
    body: JSON.stringify(input),
  });
}

export async function loginUser(input: {
  email: string;
  password: string;
}) {
  const response =
    await request<{ token: string }>("/auth/login", {
      method: "POST",
      body: JSON.stringify(input),
    });

  setToken(response.token);

  return response;
}

export async function getAccount() {
  return request<Account>("/account");
}

export async function getSubscription() {
  return request<Subscription>("/subscription");
}

export async function updateTelegramChatId(
  telegramChatId: string
) {
  return request<{ success: boolean }>(
    "/account/telegram-chat-id",
    {
      method: "PUT",
      body: JSON.stringify({ telegramChatId }),
    }
  );
}

export async function getUserBuildings() {
  return request<Building[]>("/buildings");
}

export async function getUserRooms() {
  const rooms =
    await request<Array<Partial<Room>>>("/rooms");

  return rooms.map((room) => ({
    id: Number(room.id ?? room.roomId ?? 0),
    roomId: Number(room.id ?? room.roomId ?? 0),
    buildingId: Number(room.buildingId ?? 0),
    name: String(room.roomName ?? room.name ?? ""),
    roomName: String(room.roomName ?? room.name ?? ""),
    buildingName: String(room.buildingName ?? ""),
    motion: Boolean(room.motion ?? false),
    power: Number(room.power ?? 0),
    light: Number(room.light ?? 0),
  }));
}

export async function getUserDevices() {
  return request<Device[]>("/devices");
}

export async function getUserAnomalies() {
  return request<Anomaly[]>("/anomalies/latest");
}

export async function getUserStats() {
  const [buildings, rooms, devices, anomalies] =
    await Promise.all([
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

export const getBuildings = getUserBuildings;
export const getRooms = getUserRooms;
export const getDevices = getUserDevices;

export async function getUserBuilding(id: string) {
  const buildings =
    await getUserBuildings();

  return (
    buildings.find((building) =>
      String(building.id) === String(id)
    ) ?? null
  );
}

export async function getUserBuildingRooms(
  buildingId: string
) {
  const rooms =
    await getUserRooms();

  return rooms.filter((room) =>
    String(room.buildingId) === String(buildingId)
  );
}

export async function getUserBuildingDevices(
  buildingId: string
) {
  const rooms =
    await getUserBuildingRooms(buildingId);

  const roomNames =
    new Set(rooms.map((room) => room.roomId));

  const devices =
    await getUserDevices();

  return devices.filter((device) =>
    roomNames.has(device.roomId)
  );
}

export async function getUserBuildingAnomalies(
  buildingId: string
) {
  const rooms =
    await getUserBuildingRooms(buildingId);

  const roomNames =
    new Set(rooms.map((room) => room.roomName));

  const anomalies =
    await getUserAnomalies();

  return anomalies.filter((anomaly) =>
    roomNames.has(anomaly.room)
  );
}

export async function getUserDevice(id: string) {
  const devices =
    await getUserDevices();

  return (
    devices.find((device) =>
      String(device.deviceId) === String(id)
    ) ?? null
  );
}

export async function getUserRoom(id: string) {
  const rooms =
    await getUserRooms();

  return (
    rooms.find((room) =>
      String(room.roomId) === String(id)
    ) ?? null
  );
}

export async function getUserRoomDevices(
  roomId: string
) {
  const room =
    await getUserRoom(roomId);

  if (!room) {
    return [];
  }

  const devices =
    await getUserDevices();

  return devices.filter((device) =>
    device.roomId === room.roomId
  );
}

export async function getUserRoomAnomalies(
  roomId: string
) {
  const room =
    await getUserRoom(roomId);

  if (!room) {
    return [];
  }

  const anomalies =
    await getUserAnomalies();

  return anomalies.filter((anomaly) =>
    anomaly.room === room.roomName
  );
}

export async function getPowerHistory() {
  return request<unknown[]>("/power/history");
}

export async function getTopConsumers() {
  return request<unknown[]>("/analytics/top-consumers");
}

export async function getAnomalyStatistics() {
  return request<unknown[]>("/analytics/anomalies-by-type");
}

export async function getSeverityDistribution() {
  return request<unknown[]>("/analytics/severity-distribution");
}

export async function getEnergyByRoom() {
  return request<unknown[]>("/analytics/energy-by-room");
}

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
