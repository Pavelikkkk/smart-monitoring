import { useEffect, useState } from "react";
import { useParams } from "react-router-dom";

import {
  getBuildings,
  getRooms,
  getDevices,
  getAnomalies,
} from "../services/api";

import StatCard from "../components/StatCard";
import AlertCard from "../components/AlertCard";

import PowerChart from "../components/PowerChart";
import TopConsumersChart from "../components/TopConsumersChart";
import AnomaliesChart from "../components/AnomaliesChart";
import SeverityDistributionChart from "../components/SeverityDistributionChart";

export default function BuildingAnalytics() {
  const { id } = useParams();

  const [building, setBuilding] =
    useState<any>(null);

  const [rooms, setRooms] =
    useState<any[]>([]);

  const [devices, setDevices] =
    useState<any[]>([]);

  const [anomalies, setAnomalies] =
    useState<any[]>([]);

  useEffect(() => {
    loadData();
  }, []);

  async function loadData() {
    try {
      const [
        buildingsData,
        roomsData,
        devicesData,
        anomaliesData,
      ] = await Promise.all([
        getBuildings(),
        getRooms(),
        getDevices(),
        getAnomalies(),
      ]);

      const currentBuilding =
        buildingsData.find(
          (b: any) =>
            String(b.id) === id
        );

      setBuilding(currentBuilding);

      const buildingRooms =
        roomsData.filter(
          (r: any) =>
            String(r.buildingId) === id
        );

      setRooms(buildingRooms);

      const roomNames =
        buildingRooms.map(
          (r: any) =>
            r.roomName
        );

      const buildingDevices =
        devicesData.filter(
          (d: any) =>
            roomNames.includes(
              d.roomName
            )
        );

      setDevices(buildingDevices);

      const buildingAnomalies =
        anomaliesData.filter(
          (a: any) =>
            roomNames.includes(
              a.room
            )
        );

      setAnomalies(
        buildingAnomalies
      );
    }
    catch (error) {
      console.error(error);
    }
  }

  if (!building) {
    return (
      <div>
        Loading...
      </div>
    );
  }

  return (
    <div className="space-y-8">

      {/* HEADER */}

      <div>

        <h1 className="text-5xl font-bold mb-3">
          {building.name}
        </h1>

              <p className="text-slate-500">
          Energy consumption and anomaly
          analytics for this building.
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

        <StatCard
          title="Rooms"
          value={rooms.length}
        />

        <StatCard
          title="Devices"
          value={devices.length}
        />

        <StatCard
          title="Alerts"
          value={anomalies.length}
        />

        <StatCard
          title="Status"
          value="Active"
        />

      </div>

      {/* POWER */}

      <div>

        <h2 className="text-3xl font-bold mb-4">
          Power Consumption
        </h2>

        <div
          className="
            bg-[#111827]
            border
            border-cyan-900/40
            rounded-2xl
            p-6
          "
        >
          <PowerChart />
        </div>

      </div>

      {/* CHARTS */}

      <div
        className="
          grid
          grid-cols-1
          xl:grid-cols-2
          gap-6
        "
      >

        <TopConsumersChart />

        <AnomaliesChart />

      </div>

      <div
        className="
          grid
          grid-cols-1
          gap-6
        "
      >

        <SeverityDistributionChart />

      </div>

      {/* RECENT ALERTS */}

      <div>

        <h2 className="text-3xl font-bold mb-4">
          Recent Alerts
        </h2>

        <div className="grid gap-4">

          {anomalies.length === 0 && (
            <div
              className="
                bg-[#111827]
                rounded-2xl
                p-6
                text-slate-500
              "
            >
              No anomalies detected
            </div>
          )}

          {anomalies
            .slice(0, 5)
            .map(
              (
                anomaly,
                index
              ) => (
                <AlertCard
                  key={`${anomaly.room}-${anomaly.type}-${index}`}
                  room={anomaly.room}
                  type={anomaly.type}
                  severity={
                    anomaly.severity
                  }
                  score={
                    anomaly.score
                  }
                  status={
                    anomaly.status ??
                    "ACTIVE"
                  }
                />
              )
            )}

        </div>

      </div>

    </div>
  );
}