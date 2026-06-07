import { useEffect, useState } from "react";

import { getAnomalies } from "../services/api";

export default function Anomalies() {
  const [anomalies, setAnomalies] =
    useState<any[]>([]);

  useEffect(() => {
    getAnomalies()
      .then(setAnomalies)
      .catch(console.error);
  }, []);

  const severityColor = {
    INFO: "text-orange-200",
    WARNING: "text-yellow-400",
    CRITICAL: "text-red-400",
  };

  return (
    <>
      <h1 className="text-4xl font-bold mb-8">
        Anomalies
      </h1>

      <div className="bg-slate-800 border border-slate-700 rounded-xl overflow-hidden">

        <table className="w-full">

          <thead className="bg-slate-700">
            <tr>
              <th className="text-left p-4">
                Room
              </th>

              <th className="text-left p-4">
                Type
              </th>

              <th className="text-left p-4">
                Severity
              </th>

              <th className="text-left p-4">
                Score
              </th>

              <th className="text-left p-4">
                Time
              </th>
            </tr>
          </thead>

          <tbody>

            {anomalies.map((anomaly) => (
              <tr
                key={`${anomaly.room}-${anomaly.type}-${anomaly.detectedAt}`}
                className="
                  border-t
                  border-slate-700
                  hover:bg-slate-700/30
                "
              >
                <td className="p-4">
                  {anomaly.room}
                </td>

                <td className="p-4">
                  {anomaly.type}
                </td>

                <td
                  className={`
                    p-4
                    ${
                      severityColor[
                        anomaly.severity as keyof typeof severityColor
                      ]
                    }
                  `}
                >
                  {anomaly.severity}
                </td>

                <td className="p-4">
                  {Number(anomaly.score).toFixed(3)}
                </td>

                <td className="p-4 text-slate-400">
                  {new Date(
                    anomaly.detectedAt
                  ).toLocaleString()}
                </td>

              </tr>
            ))}

          </tbody>

        </table>

      </div>
    </>
  );
}