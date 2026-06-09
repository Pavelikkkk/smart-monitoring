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
  getTopConsumers,
} from "../services/api";

export default function TopConsumersChart() {
  const [data, setData] =
    useState<any[]>([]);

  useEffect(() => {
    getTopConsumers()
      .then(setData)
      .catch(console.error);
  }, []);

  return (
    <div className="bg-[#111827] border border-cyan-700/40 rounded-2xl p-5">

      <h2 className="text-2xl font-bold mb-6">
        Top Power Consumers
      </h2>

      <div style={{ height: 300 }}>

        <ResponsiveContainer>

          <BarChart data={data}>

            <XAxis
              dataKey="roomName"
              stroke="#94a3b8"
            />

            <YAxis
              stroke="#94a3b8"
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
              fill="#22d3ee"
            />

          </BarChart>

        </ResponsiveContainer>

      </div>

    </div>
  );
}