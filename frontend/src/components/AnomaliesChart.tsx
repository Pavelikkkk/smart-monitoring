import { useEffect, useState } from "react";

import { ResponsiveContainer, PieChart, Pie, Tooltip, Cell } from "recharts";

import { getAnomalyStatistics } from "../services/api";
import type { AnomalyStatistic } from "../services/api";

const COLORS = ["#06b6d4", "#22c55e", "#f59e0b", "#ef4444"];

export default function AnomaliesChart() {
  const [data, setData] = useState<AnomalyStatistic[]>([]);

  useEffect(() => {
    getAnomalyStatistics().then(setData).catch(console.error);
  }, []);

  return (
    <div className="bg-[#111827]
          border
          border-cyan-700/40
          rounded-2xl
          p-5">
      <h2 className="text-2xl
            font-bold
            mb-6">Anomalies By Type</h2>

      <div style={{ height: 300 }}>
        <ResponsiveContainer>
          <PieChart>
            <Pie
              data={data}
              dataKey="count"
              nameKey="type"
              outerRadius={100}
              label
            >
              {data.map((_, index) => (
                <Cell key={index} fill={COLORS[index % COLORS.length]} />
              ))}
            </Pie>

            <Tooltip
              contentStyle={{
                backgroundColor: "#111827",
                border: "1px solid #164e63",
                borderRadius: "12px",
              }}
            />
          </PieChart>
        </ResponsiveContainer>
      </div>
    </div>
  );
}
