import { useEffect, useState } from "react";

import {
  ResponsiveContainer,
  BarChart,
  Bar,
  XAxis,
  YAxis,
  Tooltip,
} from "recharts";

import {
  getEnergyByRoom,
} from "../services/api";

export default function EnergyByRoomChart() {
  const [data, setData] =
    useState<any[]>([]);

  useEffect(() => {
    getEnergyByRoom()
      .then(setData)
      .catch(console.error);
  }, []);

  return (
    <div className="bg-[#111827] border border-cyan-900/40 rounded-2xl p-6">

      <h2 className="text-2xl font-bold mb-6">
        Energy By Room
      </h2>

      <div style={{ height: 350 }}>

        <ResponsiveContainer>

          <BarChart
            data={data}
            layout="vertical"
          >

            <XAxis type="number" />

            <YAxis
              type="category"
              dataKey="roomName"
            />

            <Tooltip
              contentStyle={{
                backgroundColor: "#111827",
                border: "1px solid #164e63",
                borderRadius: "12px"
              }}
            />

            <Bar
              dataKey="power"
              fill="#60a5fa"
            />

          </BarChart>

        </ResponsiveContainer>

      </div>

    </div>
  );
}