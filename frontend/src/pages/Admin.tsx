import { useEffect, useMemo, useState } from "react";

import {
  type AdminOverview,
  createAdminBuilding,
  createAdminDevice,
  createAdminRoom,
  getAccount,
  getAdminOverview,
} from "../services/api";
import {
  CreateBuildingForm,
  CreateDeviceForm,
  CreateRoomForm,
} from "./admin/AdminForms";
import { AdminUsersTable } from "./admin/AdminUsersTable";

export default function Admin() {
  const [overview, setOverview] = useState<AdminOverview | null>(null);
  const [error, setError] = useState("");
  const [status, setStatus] = useState("");
  const [selectedOrganizationId, setSelectedOrganizationId] = useState(0);
  const [buildingName, setBuildingName] = useState("");
  const [buildingAddress, setBuildingAddress] = useState("");
  const [buildingDescription, setBuildingDescription] = useState("");
  const [selectedBuildingId, setSelectedBuildingId] = useState(0);
  const [roomName, setRoomName] = useState("");
  const [roomType, setRoomType] = useState("ROOM");
  const [floorNumber, setFloorNumber] = useState(1);
  const [selectedRoomId, setSelectedRoomId] = useState(0);
  const [deviceId, setDeviceId] = useState("");
  const [deviceName, setDeviceName] = useState("");
  const [deviceModel, setDeviceModel] = useState("ESP32");
  const [firmwareVersion, setFirmwareVersion] = useState("1.0.0");

  async function fetchAdminOverview() {
    const account = await getAccount();

    if (account.role !== "ADMIN") {
      throw new Error("Admin role is required");
    }

    return getAdminOverview();
  }

  function applyAdminOverview(data: AdminOverview) {
    setOverview(data);

    const firstUser = data.users.find((user) => user.organizationId > 0);

    if (firstUser && !selectedOrganizationId) {
      setSelectedOrganizationId(firstUser.organizationId);
    }
  }

  async function loadAdmin() {
    setError("");

    try {
      const data = await fetchAdminOverview();
      applyAdminOverview(data);
    } catch (err) {
      setError(
        err instanceof Error ? err.message : "Failed to load admin data",
      );
    }
  }

  useEffect(() => {
    async function loadInitialAdmin() {
      try {
        const data = await fetchAdminOverview();

        setOverview(data);

        const firstUser = data.users.find((user) => user.organizationId > 0);

        if (firstUser) {
          setSelectedOrganizationId(firstUser.organizationId);
        }
      } catch (err) {
        setError(
          err instanceof Error ? err.message : "Failed to load admin data",
        );
      }
    }

    loadInitialAdmin();
  }, []);

  const organizationBuildings = useMemo(() => {
    if (!overview) {
      return [];
    }

    return overview.buildings.filter(
      (building) => building.organizationId === selectedOrganizationId,
    );
  }, [overview, selectedOrganizationId]);

  const buildingRooms = useMemo(() => {
    if (!overview) {
      return [];
    }

    return overview.rooms.filter(
      (room) => room.buildingId === selectedBuildingId,
    );
  }, [overview, selectedBuildingId]);

  async function handleCreateBuilding(event: React.FormEvent) {
    event.preventDefault();
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

  async function handleCreateRoom(event: React.FormEvent) {
    event.preventDefault();
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

  async function handleCreateDevice(event: React.FormEvent) {
    event.preventDefault();
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
      <h1
        className="text-5xl
        font-bold"
      >
        Admin
      </h1>

      {status && (
        <div
          className="bg-cyan-500/10
          border
          border-cyan-700/40
          rounded-2xl
          p-4
          text-cyan-300"
        >
          {status}
        </div>
      )}

      <AdminUsersTable overview={overview} />

      <div
        className="grid
        grid-cols-1
        xl:grid-cols-3
        gap-5"
      >
        <CreateBuildingForm
          overview={overview}
          selectedOrganizationId={selectedOrganizationId}
          buildingName={buildingName}
          buildingAddress={buildingAddress}
          buildingDescription={buildingDescription}
          onSubmit={handleCreateBuilding}
          onOrganizationChange={setSelectedOrganizationId}
          onBuildingNameChange={setBuildingName}
          onBuildingAddressChange={setBuildingAddress}
          onBuildingDescriptionChange={setBuildingDescription}
        />

        <CreateRoomForm
          buildings={organizationBuildings}
          selectedBuildingId={selectedBuildingId}
          roomName={roomName}
          roomType={roomType}
          floorNumber={floorNumber}
          onSubmit={handleCreateRoom}
          onBuildingChange={setSelectedBuildingId}
          onRoomNameChange={setRoomName}
          onRoomTypeChange={setRoomType}
          onFloorNumberChange={setFloorNumber}
        />

        <CreateDeviceForm
          rooms={buildingRooms}
          selectedRoomId={selectedRoomId}
          deviceId={deviceId}
          deviceName={deviceName}
          deviceModel={deviceModel}
          firmwareVersion={firmwareVersion}
          onSubmit={handleCreateDevice}
          onRoomChange={setSelectedRoomId}
          onDeviceIdChange={setDeviceId}
          onDeviceNameChange={setDeviceName}
          onDeviceModelChange={setDeviceModel}
          onFirmwareVersionChange={setFirmwareVersion}
        />
      </div>
    </div>
  );
}
