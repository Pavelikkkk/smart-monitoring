import { useEffect, useState } from "react";

import {
  ResponsiveContainer,
  PieChart,
  Pie,
  Tooltip,
  Cell,
} from "recharts";

import {
  getSeverityDistribution,
} from "../services/api";

const COLORS = [
  "#22c55e",
  "#f59e0b",
  "#ef4444",
];

export default function SeverityDistributionChart() {
  const [data, setData] =
    useState<any[]>([]);

  useEffect(() => {
    getSeverityDistribution()
      .then(setData)
      .catch(console.error);
  }, []);

  return (
    <div className="bg-slate-800 border border-slate-700 rounded-xl p-6">

      <h2 className="text-2xl font-bold mb-6">
        Severity Distribution
      </h2>

      <div style={{ height: 350 }}>

        <ResponsiveContainer>

          <PieChart>

            <Pie
              data={data}
              dataKey="count"
              nameKey="severity"
              outerRadius={110}
              label
            >
              {data.map((_, index) => (
                <Cell
                  key={index}
                  fill={
                    COLORS[
                      index %
                        COLORS.length
                    ]
                  }
                />
              ))}
            </Pie>

            <Tooltip />

          </PieChart>

        </ResponsiveContainer>

      </div>

    </div>
  );
}