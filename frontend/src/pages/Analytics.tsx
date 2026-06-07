import { useEffect, useState } from "react";

import {
  getStats,
  getHealth,
  getAnomalies,
  getRooms,
} from "../services/api";

import StatCard from "../components/StatCard";
import AlertCard from "../components/AlertCard";
import StatusBadge from "../components/StatusBadge";
import PowerChart from "../components/PowerChart";
import TopRooms from "../components/TopRooms";

import TopConsumersChart from "../components/TopConsumersChart";
import AnomaliesChart from "../components/AnomaliesChart";
import EnergyByRoomChart from "../components/EnergyByRoomChart";
import SeverityDistributionChart from "../components/SeverityDistributionChart";

export default function Analytics() {
  const [stats, setStats] = useState({
    rooms: 0,
    alerts: 0,
    mlAlerts: 0,
    mqttOnline: false,
  });

  const [health, setHealth] = useState({
    mqtt: false,
    database: false,
    telegram: false,
    detector: false,
  });

  const [anomalies, setAnomalies] =
    useState<any[]>([]);

  const [rooms, setRooms] =
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
        roomsData,
      ] = await Promise.all([
        getStats(),
        getHealth(),
        getAnomalies(),
        getRooms(),
      ]);

      setStats(statsData);

      setHealth(healthData);

      setAnomalies(
        anomaliesData.slice(0, 5)
      );

      const sortedRooms =
        [...roomsData]
          .sort(
            (a: any, b: any) =>
              (b.power ?? 0) -
              (a.power ?? 0)
          )
          .slice(0, 5);

      setRooms(sortedRooms);
    }
    catch (error) {
      console.error(error);
    }
  }

  return (
    <>
      <h1 className="text-4xl font-bold mb-8">
        Analytics Dashboard
      </h1>

      {/* STATS */}

      <div className="grid grid-cols-1 sm:grid-cols-2 xl:grid-cols-4 gap-4 mb-8">

        <StatCard
          title="Rooms"
          value={stats.rooms}
        />

        <StatCard
          title="Alerts"
          value={stats.alerts}
        />

        <StatCard
          title="ML Alerts"
          value={stats.mlAlerts}
        />

        <StatCard
          title="MQTT"
          value={
            stats.mqttOnline
              ? "Online"
              : "Offline"
          }
        />

      </div>

      {/* ANOMALIES + TOP ROOMS */}

      <div className="grid grid-cols-1 lg:grid-cols-2 gap-6 mb-8">

        <div>

          <h2 className="text-3xl font-bold mb-4">
            Latest Anomalies
          </h2>

          <div className="grid gap-4">

            {anomalies.length === 0 && (
              <div className="bg-slate-800 rounded-xl p-6 text-slate-400">
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

        <TopRooms rooms={rooms} />

      </div>

      {/* POWER HISTORY */}

      <h2 className="text-3xl font-bold mb-4">
        Power Consumption
      </h2>

      <div className="bg-slate-800 border border-slate-700 rounded-xl p-6 mb-8">

        <PowerChart />

      </div>

      {/* ANALYTICS BLOCK 1 */}

      <div className="grid grid-cols-1 xl:grid-cols-2 gap-6 mb-8">

        <TopConsumersChart />

        <AnomaliesChart />

      </div>

      {/* ANALYTICS BLOCK 2 */}

      <div className="grid grid-cols-1 xl:grid-cols-2 gap-6 mb-8">

        <EnergyByRoomChart />

        <SeverityDistributionChart />

      </div>

      {/* SYSTEM STATUS */}

      <h2 className="text-3xl font-bold mb-4">
        System Status
      </h2>

      <div className="bg-slate-800 border border-slate-700 rounded-xl p-6">

        <div className="flex justify-between py-2">
          <span>MQTT Broker</span>
          <StatusBadge online={health.mqtt} />
        </div>

        <div className="flex justify-between py-2">
          <span>TimescaleDB</span>
          <StatusBadge online={health.database} />
        </div>

        <div className="flex justify-between py-2">
          <span>Hybrid Detector</span>
          <StatusBadge online={health.detector} />
        </div>

        <div className="flex justify-between py-2">
          <span>Telegram</span>
          <StatusBadge online={health.telegram} />
        </div>

      </div>

    </>
  );
}