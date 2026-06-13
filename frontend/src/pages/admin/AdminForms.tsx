import type { AdminBuilding, AdminOverview, AdminRoom } from "../../services/api";

const fieldClass = `w-full
px-4
py-3
rounded-xl
bg-[#020617]
border
border-slate-700`;

const formClass = `bg-[#111827]
border
border-cyan-700/40
rounded-3xl
p-5
space-y-4`;

const buttonClass = `w-full
py-3
rounded-xl
bg-cyan-500
text-slate-900
font-semibold
disabled:opacity-50`;

type CreateBuildingFormProps = {
  overview: AdminOverview;
  selectedOrganizationId: number;
  buildingName: string;
  buildingAddress: string;
  buildingDescription: string;
  onSubmit: (event: React.FormEvent) => void;
  onOrganizationChange: (value: number) => void;
  onBuildingNameChange: (value: string) => void;
  onBuildingAddressChange: (value: string) => void;
  onBuildingDescriptionChange: (value: string) => void;
};

export function CreateBuildingForm({
  overview,
  selectedOrganizationId,
  buildingName,
  buildingAddress,
  buildingDescription,
  onSubmit,
  onOrganizationChange,
  onBuildingNameChange,
  onBuildingAddressChange,
  onBuildingDescriptionChange,
}: CreateBuildingFormProps) {
  return (
    <form
      onSubmit={onSubmit}
      className={formClass}
    >
      <h2
        className="text-2xl
        font-bold"
      >
        Create Building
      </h2>

      <select
        value={selectedOrganizationId}
        onChange={(event) => onOrganizationChange(Number(event.target.value))}
        className={fieldClass}
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
        onChange={(event) => onBuildingNameChange(event.target.value)}
        className={fieldClass}
      />

      <input
        placeholder="Address"
        value={buildingAddress}
        onChange={(event) => onBuildingAddressChange(event.target.value)}
        className={fieldClass}
      />

      <input
        placeholder="Description"
        value={buildingDescription}
        onChange={(event) => onBuildingDescriptionChange(event.target.value)}
        className={fieldClass}
      />

      <button className={buttonClass}>Create</button>
    </form>
  );
}

type CreateRoomFormProps = {
  buildings: AdminBuilding[];
  selectedBuildingId: number;
  roomName: string;
  roomType: string;
  floorNumber: number;
  onSubmit: (event: React.FormEvent) => void;
  onBuildingChange: (value: number) => void;
  onRoomNameChange: (value: string) => void;
  onRoomTypeChange: (value: string) => void;
  onFloorNumberChange: (value: number) => void;
};

export function CreateRoomForm({
  buildings,
  selectedBuildingId,
  roomName,
  roomType,
  floorNumber,
  onSubmit,
  onBuildingChange,
  onRoomNameChange,
  onRoomTypeChange,
  onFloorNumberChange,
}: CreateRoomFormProps) {
  return (
    <form
      onSubmit={onSubmit}
      className={formClass}
    >
      <h2
        className="text-2xl
        font-bold"
      >
        Create Room
      </h2>

      <select
        value={selectedBuildingId}
        onChange={(event) => onBuildingChange(Number(event.target.value))}
        className={fieldClass}
      >
        <option value={0}>Select building</option>
        {buildings.map((building) => (
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
        onChange={(event) => onRoomNameChange(event.target.value)}
        className={fieldClass}
      />

      <input
        required
        placeholder="Room type"
        value={roomType}
        onChange={(event) => onRoomTypeChange(event.target.value)}
        className={fieldClass}
      />

      <input
        type="number"
        value={floorNumber}
        onChange={(event) => onFloorNumberChange(Number(event.target.value))}
        className={fieldClass}
      />

      <button
        disabled={!selectedBuildingId}
        className={buttonClass}
      >
        Create
      </button>
    </form>
  );
}

type CreateDeviceFormProps = {
  rooms: AdminRoom[];
  selectedRoomId: number;
  deviceId: string;
  deviceName: string;
  deviceModel: string;
  firmwareVersion: string;
  onSubmit: (event: React.FormEvent) => void;
  onRoomChange: (value: number) => void;
  onDeviceIdChange: (value: string) => void;
  onDeviceNameChange: (value: string) => void;
  onDeviceModelChange: (value: string) => void;
  onFirmwareVersionChange: (value: string) => void;
};

export function CreateDeviceForm({
  rooms,
  selectedRoomId,
  deviceId,
  deviceName,
  deviceModel,
  firmwareVersion,
  onSubmit,
  onRoomChange,
  onDeviceIdChange,
  onDeviceNameChange,
  onDeviceModelChange,
  onFirmwareVersionChange,
}: CreateDeviceFormProps) {
  return (
    <form
      onSubmit={onSubmit}
      className={formClass}
    >
      <h2
        className="text-2xl
        font-bold"
      >
        Create Device
      </h2>

      <select
        value={selectedRoomId}
        onChange={(event) => onRoomChange(Number(event.target.value))}
        className={fieldClass}
      >
        <option value={0}>Select room</option>
        {rooms.map((room) => (
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
        onChange={(event) => onDeviceIdChange(event.target.value)}
        className={fieldClass}
      />

      <input
        required
        placeholder="Device name"
        value={deviceName}
        onChange={(event) => onDeviceNameChange(event.target.value)}
        className={fieldClass}
      />

      <input
        value={deviceModel}
        onChange={(event) => onDeviceModelChange(event.target.value)}
        className={fieldClass}
      />

      <input
        value={firmwareVersion}
        onChange={(event) => onFirmwareVersionChange(event.target.value)}
        className={fieldClass}
      />

      <button
        disabled={!selectedRoomId}
        className={buttonClass}
      >
        Create
      </button>
    </form>
  );
}
