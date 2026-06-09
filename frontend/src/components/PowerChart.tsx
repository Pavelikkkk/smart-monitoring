import { useEffect, useState } from "react";

import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  Tooltip,
  ResponsiveContainer,
  CartesianGrid,
} from "recharts";

import { getPowerHistory } from "../services/api";

type PowerPoint = {
  time: string;
  power: number;
};

export default function PowerChart() {
  const [data, setData] = useState<PowerPoint[]>([]);

  useEffect(() => {
    getPowerHistory()
      .then((history) => {
        const formatted = history.map(
          (item: any) => ({
            time: new Date(
              item.time
            ).toLocaleTimeString([], {
              hour: "2-digit",
              minute: "2-digit",
            }),

            power: Number(item.power),
          })
        );

        setData(formatted);
      })
      .catch(console.error);
  }, []);

  const current =
    data.length > 0
      ? data[data.length - 1].power
      : 0;

  const peak =
    data.length > 0
      ? Math.max(
          ...data.map((x) => x.power)
        )
      : 0;

  const average =
    data.length > 0
      ? Math.round(
          data.reduce(
            (sum, item) =>
              sum + item.power,
            0
          ) / data.length
        )
      : 0;

  return (
    <div className="w-full">
      {/* Header */}

      <div className="flex flex-col lg:flex-row lg:justify-between lg:items-center mb-6 gap-4">

        <div>
          <h3 className="text-xl font-semibold text-white">
            Energy Usage Analytics
          </h3>

          <p className="text-slate-300 text-sm">
            Last 24 hours power consumption
          </p>
        </div>

      </div>

      {/* Metrics */}

      <div className="grid grid-cols-3 gap-4 mb-6">

        <div className="bg-[#020617] rounded-lg p-4">
          <p className="text-slate-300 text-sm">
            Current
          </p>

          <h3 className="text-2xl font-bold text-cyan-400">
            {current.toFixed(2)}
          </h3>
        </div>

        <div className="bg-[#020617] rounded-lg p-4">
          <p className="text-slate-300 text-sm">
            Average
          </p>

          <h3 className="text-2xl font-bold text-emerald-400">
            {average}
          </h3>
        </div>

        <div className="bg-[#020617] rounded-lg p-4">
          <p className="text-slate-300 text-sm">
            Peak
          </p>

          <h3 className="text-2xl font-bold text-orange-400">
            {peak.toFixed(2)}
          </h3>
        </div>

      </div>

      {/* Chart */}

      <div style={{ height: 350 }}>
        <ResponsiveContainer
          width="100%"
          height="100%"
        >
          <LineChart data={data}>

            <CartesianGrid
              stroke="#334155"
              strokeDasharray="3 3"
            />

            <XAxis
              dataKey="time"
              stroke="#94a3b8"
            />

            <YAxis
              stroke="#94a3b8"
            />

            <Tooltip
              contentStyle={{
                backgroundColor: "#0f172a",
                border: "1px solid #334155",
                borderRadius: "10px",
              }}
              labelStyle={{
                color: "#ffffff",
              }}
            />

            <Line
              type="monotone"
              dataKey="power"
              stroke="#06b6d4"
              strokeWidth={4}
              dot={false}
              activeDot={{
                r: 8,
                fill: "#06b6d4",
              }}
            />

          </LineChart>
        </ResponsiveContainer>
      </div>
    </div>
  );
}