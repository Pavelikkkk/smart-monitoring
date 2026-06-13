import { useEffect, useState } from "react";

import {
  getUserStats,
  getUserAnomalies,
  getUserDevices,
} from "../services/api";
import type { Anomaly, Device } from "../services/api";

import StatCard from "../components/StatCard";
import AlertCard from "../components/AlertCard";

import PowerChart from "../components/PowerChart";
import TopConsumersChart from "../components/TopConsumersChart";
import AnomaliesChart from "../components/AnomaliesChart";
import SeverityDistributionChart from "../components/SeverityDistributionChart";
import EnergyByRoomChart from "../components/EnergyByRoomChart";

export default function Analytics() {
  const [stats, setStats] = useState({
    buildings: 0,
    rooms: 0,
    devices: 0,
    anomalies: 0,
  });

  const [devices, setDevices] = useState<Device[]>([]);

  const [anomalies, setAnomalies] = useState<Anomaly[]>([]);

  useEffect(() => {
    async function loadAnalytics() {
      try {
        const [statsData, anomaliesData, devicesData] = await Promise.all([
          getUserStats(),
          getUserAnomalies(),
          getUserDevices(),
        ]);

        setStats(statsData);

        setDevices(devicesData);

        setAnomalies(anomaliesData.slice(0, 5));
      } catch (error) {
        console.error(error);
      }
    }

    loadAnalytics();
  }, []);

  return (
    <div className="space-y-8">
      {/* HEADER */}

      <div>
        <h1 className="text-5xl
              font-bold
              mb-3">Analytics</h1>

        <p className="text-slate-300">
          Monitor energy consumption, device activity and anomaly statistics
          across your buildings.
        </p>
      </div>

      {/* STATS */}

      <div
        className="grid
              grid-cols-1
              md:grid-cols-2
              xl:grid-cols-4
              gap-4"
      >
        <StatCard title="Buildings" value={stats.buildings} />

        <StatCard title="Rooms" value={stats.rooms} />

        <StatCard title="Devices" value={devices.length} />

        <StatCard title="Anomalies" value={stats.anomalies} />
      </div>

      {/* POWER */}

      <div>
        <h2 className="text-3xl
              font-bold
              mb-4">Power Consumption</h2>

        <div
          className="bg-[#111827]
                border
                border-cyan-700/40
                rounded-2xl
                p-5"
        >
          <PowerChart />
        </div>
      </div>

      {/* ENERGY BY ROOM */}

      <div>
        <h2 className="text-3xl
              font-bold
              mb-4">Energy by Room</h2>

        <EnergyByRoomChart />
      </div>

      {/* CHARTS */}

      <div
        className="grid
              grid-cols-1
              xl:grid-cols-2
              gap-5"
      >
        <TopConsumersChart />

        <AnomaliesChart />
      </div>

      <SeverityDistributionChart />

      {/* LATEST ANOMALIES */}

      <div>
        <h2 className="text-3xl
              font-bold
              mb-4">Latest Anomalies</h2>

        <div className="grid
              gap-4">
          {anomalies.length === 0 && (
            <div
              className="bg-[#111827]
                    rounded-2xl
                    p-5
                    text-slate-300"
            >
              No anomalies detected
            </div>
          )}

          {anomalies.map((anomaly, index) => (
            <AlertCard
              key={`${anomaly.room}-${anomaly.type}-${index}`}
              room={anomaly.room}
              type={anomaly.type}
              severity={anomaly.severity}
              score={anomaly.score}
              status={anomaly.status ?? "ACTIVE"}
            />
          ))}
        </div>
      </div>
    </div>
  );
}
