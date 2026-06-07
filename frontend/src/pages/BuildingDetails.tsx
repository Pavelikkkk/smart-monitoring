import { useParams } from "react-router-dom";
import { useEffect, useState } from "react";

import {
  getBuildings,
  getRooms,
} from "../services/api";

export default function BuildingDetails() {
  const { id } = useParams();

  const [building, setBuilding] =
    useState<any>(null);

  const [rooms, setRooms] =
    useState<any[]>([]);

  useEffect(() => {
    loadData();
  }, []);

  async function loadData() {
    try {
      const [
        buildings,
        roomsData,
      ] = await Promise.all([
        getBuildings(),
        getRooms(),
      ]);

      const currentBuilding =
        buildings.find(
          (b: any) =>
            String(b.id) === id
        );

      setBuilding(currentBuilding);

      setRooms(
        roomsData.filter(
          (r: any) =>
            String(r.buildingId) === id
        )
      );
    }
    catch (error) {
      console.error(error);
    }
  }

  if (!building)
  {
    return (
      <div>
        Loading...
      </div>
    );
  }

  return (
    <>
      <h1 className="text-4xl font-bold mb-2">
        {building.name}
      </h1>

      <p className="text-slate-400 mb-8">
        {building.address}
      </p>

      <div className="grid grid-cols-3 gap-4 mb-8">

        <div className="bg-slate-800 rounded-xl p-6">
          <p className="text-slate-400">
            Rooms
          </p>

                  <h2 className="text-3xl font-bold text-orange-200">
            {rooms.length}
          </h2>
        </div>

        <div className="bg-slate-800 rounded-xl p-6">
          <p className="text-slate-400">
            Devices
          </p>

          <h2 className="text-3xl font-bold text-green-400">
            -
          </h2>
        </div>

        <div className="bg-slate-800 rounded-xl p-6">
          <p className="text-slate-400">
            Active Alerts
          </p>

          <h2 className="text-3xl font-bold text-red-400">
            -
          </h2>
        </div>

      </div>

      <h2 className="text-3xl font-bold mb-4">
        Rooms
      </h2>

      <div className="grid gap-4">

        {rooms.map((room) => (
          <div
            key={room.id}
            className="
              bg-slate-800
              border
              border-slate-700
              rounded-xl
              p-5
            "
          >
            <div className="flex justify-between">

              <div>

                <h3 className="text-xl font-semibold">
                  {room.roomName}
                </h3>

                <p className="text-slate-400">
                  {room.roomType}
                </p>

              </div>

              <div className="text-slate-400">
                Floor {room.floorNumber}
              </div>

            </div>
          </div>
        ))}

      </div>
    </>
  );
}