import { useEffect, useState } from "react";

import {
  getStats,
  getHealth,
  getAnomalies,
  //getRooms,
  getDevices,
} from "../services/api";

import StatCard from "../components/StatCard";
import AlertCard from "../components/AlertCard";
import StatusBadge from "../components/StatusBadge";
import PowerChart from "../components/PowerChart";

import TopConsumersChart from "../components/TopConsumersChart";
import AnomaliesChart from "../components/AnomaliesChart";
import SeverityDistributionChart from "../components/SeverityDistributionChart";

export default function Analytics() {
  const [stats, setStats] = useState({
    rooms: 0,
    alerts: 0,
    mlAlerts: 0,
    mqttOnline: false,
  });

  const [devices, setDevices] =
    useState<any[]>([]);

  const [health, setHealth] = useState({
    mqtt: false,
    database: false,
    telegram: false,
    detector: false,
  });

  const [anomalies, setAnomalies] =
    useState<any[]>([]);

  useEffect(() => {
    loadAnalytics();
  }, []);

  async function loadAnalytics() {
    try {
      const [
        statsData,
        healthData,
        anomaliesData,
        devicesData,
      ] = await Promise.all([
        getStats(),
        getHealth(),
        getAnomalies(),
        getDevices(),
      ]);

      setStats(statsData);

      setHealth(healthData);

      setDevices(devicesData);

      setAnomalies(
        anomaliesData.slice(0, 5)
      );
    }
    catch (error) {
      console.error(error);
    }
  }

  return (
    <div className="space-y-8">

      {/* HEADER */}

      <div>

        <h1 className="text-5xl font-bold mb-3">
          Analytics Dashboard
        </h1>

        <p className="text-slate-500">
          Real-time energy monitoring and
          anomaly detection analytics.
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
          value={stats.rooms}
        />

        <StatCard
          title="Devices"
          value={devices.length}
        />

        <StatCard
          title="Alerts"
          value={stats.alerts}
        />

        <StatCard
          title="Server"
          value={
            stats.mqttOnline
              ? "Online"
              : "Offline"
          }
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
          xl:grid-cols-1
          gap-6
        "
      >

        <SeverityDistributionChart />

      </div>

      {/* SYSTEM */}

      <div>

        <h2 className="text-3xl font-bold mb-4">
          System Health
        </h2>

        <div
          className="
            grid
            grid-cols-1
            md:grid-cols-2
            xl:grid-cols-4
            gap-4
          "
        >

          <div className="bg-[#111827] rounded-2xl p-6">

            <p className="text-slate-500 mb-2">
              Database
            </p>

            <StatusBadge
              online={health.database}
            />

          </div>

          <div className="bg-[#111827] rounded-2xl p-6">

            <p className="text-slate-500 mb-2">
              ML Engine
            </p>

            <StatusBadge
              online={health.detector}
            />

          </div>

          <div className="bg-[#111827] rounded-2xl p-6">

            <p className="text-slate-500 mb-2">
              Notifications
            </p>

            <StatusBadge
              online={health.telegram}
            />

          </div>

        </div>

      </div>

      {/* LATEST ANOMALIES */}

      <div>

        <h2 className="text-3xl font-bold mb-4">
          Latest Anomalies
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

          {anomalies.map(
            (anomaly, index) => (
              <AlertCard
                key={`${anomaly.room}-${anomaly.type}-${index}`}
                room={anomaly.room}
                type={anomaly.type}
                severity={anomaly.severity}
                score={anomaly.score}
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