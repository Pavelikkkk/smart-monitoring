import { useEffect, useMemo, useState } from "react";

import {
  type AdminOverview,
  createAdminBuilding,
  createAdminDevice,
  createAdminRoom,
  getAccount,
  getAdminOverview,
} from "../services/api";

export default function Admin() {
  const [overview, setOverview] =
    useState<AdminOverview | null>(null);

  const [error, setError] =
    useState("");

  const [status, setStatus] =
    useState("");

  const [selectedOrganizationId, setSelectedOrganizationId] =
    useState(0);

  const [buildingName, setBuildingName] =
    useState("");

  const [buildingAddress, setBuildingAddress] =
    useState("");

  const [buildingDescription, setBuildingDescription] =
    useState("");

  const [selectedBuildingId, setSelectedBuildingId] =
    useState(0);

  const [roomName, setRoomName] =
    useState("");

  const [roomType, setRoomType] =
    useState("ROOM");

  const [floorNumber, setFloorNumber] =
    useState(1);

  const [selectedRoomId, setSelectedRoomId] =
    useState(0);

  const [deviceId, setDeviceId] =
    useState("");

  const [deviceName, setDeviceName] =
    useState("");

  const [deviceModel, setDeviceModel] =
    useState("ESP32");

  const [firmwareVersion, setFirmwareVersion] =
    useState("1.0.0");

  async function loadAdmin() {
    setError("");

    try {
      const account =
        await getAccount();

      if (account.role !== "ADMIN") {
        setError("Admin role is required");
        return;
      }

      const data =
        await getAdminOverview();

      setOverview(data);

      const firstUser =
        data.users.find(
          (user) => user.organizationId > 0
        );

      if (firstUser && !selectedOrganizationId) {
        setSelectedOrganizationId(
          firstUser.organizationId
        );
      }
    } catch (err) {
      setError(
        err instanceof Error
          ? err.message
          : "Failed to load admin data"
      );
    }
  }

  useEffect(() => {
    loadAdmin();
  }, []);

  const organizationBuildings =
    useMemo(() => {
      if (!overview) {
        return [];
      }

      return overview.buildings.filter(
        (building) =>
          building.organizationId ===
          selectedOrganizationId
      );
    }, [overview, selectedOrganizationId]);

  const buildingRooms =
    useMemo(() => {
      if (!overview) {
        return [];
      }

      return overview.rooms.filter(
        (room) =>
          room.buildingId === selectedBuildingId
      );
    }, [overview, selectedBuildingId]);

  async function handleCreateBuilding(
    e: React.FormEvent
  ) {
    e.preventDefault();
    setStatus("");

    await createAdminBuilding({
      organizationId: selectedOrganizationId,
      name: buildingName,
      address: buildingAddress,
      description: buildingDescription,
    });

    setBuildingName("");
    setBuildingAddress("");
    setBuildingDescription("");
    setStatus("Building created");
    await loadAdmin();
  }

  async function handleCreateRoom(
    e: React.FormEvent
  ) {
    e.preventDefault();
    setStatus("");

    await createAdminRoom({
      buildingId: selectedBuildingId,
      roomName,
      roomType,
      floorNumber,
    });

    setRoomName("");
    setRoomType("ROOM");
    setFloorNumber(1);
    setStatus("Room created");
    await loadAdmin();
  }

  async function handleCreateDevice(
    e: React.FormEvent
  ) {
    e.preventDefault();
    setStatus("");

    await createAdminDevice({
      roomId: selectedRoomId,
      deviceId,
      deviceName,
      deviceModel,
      firmwareVersion,
    });

    setDeviceId("");
    setDeviceName("");
    setStatus("Device created");
    await loadAdmin();
  }

  if (error) {
    return <div className="text-red-400">{error}</div>;
  }

  if (!overview) {
    return <div className="text-slate-300">Loading admin...</div>;
  }

  return (
    <div className="space-y-8">
      <h1 className="text-5xl font-bold">
        Admin
      </h1>

      {status && (
        <div className="bg-cyan-500/10 border border-cyan-700/40 rounded-2xl p-4 text-cyan-300">
          {status}
        </div>
      )}

      <div className="bg-[#111827] border border-cyan-700/40 rounded-3xl p-5 overflow-x-auto">
        <h2 className="text-3xl font-bold mb-5">
          Users & Workspaces
        </h2>

        <table className="w-full text-left">
          <thead className="text-slate-300">
            <tr>
              <th className="py-3">User</th>
              <th>Email</th>
              <th>Role</th>
              <th>Plan</th>
              <th>Workspace</th>
              <th>Data</th>
            </tr>
          </thead>

          <tbody>
            {overview.users.map((user) => (
              <tr
                key={user.id}
                className="border-t border-slate-800"
              >
                <td className="py-3">{user.username}</td>
                <td>{user.email}</td>
                <td>{user.role}</td>
                <td>{user.plan}</td>
                <td>{user.organizationName}</td>
                <td>
                  {user.buildingsCount} buildings,{" "}
                  {user.roomsCount} rooms,{" "}
                  {user.devicesCount} devices
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>

      <div className="grid grid-cols-1 xl:grid-cols-3 gap-5">
        <form
          onSubmit={handleCreateBuilding}
          className="bg-[#111827] border border-cyan-700/40 rounded-3xl p-5 space-y-4"
        >
          <h2 className="text-2xl font-bold">
            Create Building
          </h2>

          <select
            value={selectedOrganizationId}
            onChange={(e) =>
              setSelectedOrganizationId(
                Number(e.target.value)
              )
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          >
            {overview.users
              .filter((user) => user.organizationId > 0)
              .map((user) => (
                <option
                  key={user.id}
                  value={user.organizationId}
                >
                  {user.username} - {user.organizationName}
                </option>
              ))}
          </select>

          <input
            required
            placeholder="Building name"
            value={buildingName}
            onChange={(e) =>
              setBuildingName(e.target.value)
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          />

          <input
            placeholder="Address"
            value={buildingAddress}
            onChange={(e) =>
              setBuildingAddress(e.target.value)
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          />

          <input
            placeholder="Description"
            value={buildingDescription}
            onChange={(e) =>
              setBuildingDescription(e.target.value)
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          />

          <button className="w-full py-3 rounded-xl bg-cyan-500 text-slate-900 font-semibold">
            Create
          </button>
        </form>

        <form
          onSubmit={handleCreateRoom}
          className="bg-[#111827] border border-cyan-700/40 rounded-3xl p-5 space-y-4"
        >
          <h2 className="text-2xl font-bold">
            Create Room
          </h2>

          <select
            value={selectedBuildingId}
            onChange={(e) =>
              setSelectedBuildingId(
                Number(e.target.value)
              )
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          >
            <option value={0}>Select building</option>
            {organizationBuildings.map((building) => (
              <option
                key={building.id}
                value={building.id}
              >
                {building.name}
              </option>
            ))}
          </select>

          <input
            required
            placeholder="Room name"
            value={roomName}
            onChange={(e) =>
              setRoomName(e.target.value)
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          />

          <input
            required
            placeholder="Room type"
            value={roomType}
            onChange={(e) =>
              setRoomType(e.target.value)
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          />

          <input
            type="number"
            value={floorNumber}
            onChange={(e) =>
              setFloorNumber(Number(e.target.value))
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          />

          <button
            disabled={!selectedBuildingId}
            className="w-full py-3 rounded-xl bg-cyan-500 text-slate-900 font-semibold disabled:opacity-50"
          >
            Create
          </button>
        </form>

        <form
          onSubmit={handleCreateDevice}
          className="bg-[#111827] border border-cyan-700/40 rounded-3xl p-5 space-y-4"
        >
          <h2 className="text-2xl font-bold">
            Create Device
          </h2>

          <select
            value={selectedRoomId}
            onChange={(e) =>
              setSelectedRoomId(
                Number(e.target.value)
              )
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          >
            <option value={0}>Select room</option>
            {buildingRooms.map((room) => (
              <option
                key={room.id}
                value={room.id}
              >
                {room.roomName}
              </option>
            ))}
          </select>

          <input
            required
            placeholder="Device id"
            value={deviceId}
            onChange={(e) =>
              setDeviceId(e.target.value)
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          />

          <input
            required
            placeholder="Device name"
            value={deviceName}
            onChange={(e) =>
              setDeviceName(e.target.value)
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          />

          <input
            value={deviceModel}
            onChange={(e) =>
              setDeviceModel(e.target.value)
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          />

          <input
            value={firmwareVersion}
            onChange={(e) =>
              setFirmwareVersion(e.target.value)
            }
            className="w-full px-4 py-3 rounded-xl bg-[#020617] border border-slate-700"
          />

          <button
            disabled={!selectedRoomId}
            className="w-full py-3 rounded-xl bg-cyan-500 text-slate-900 font-semibold disabled:opacity-50"
          >
            Create
          </button>
        </form>
      </div>
    </div>
  );
}
