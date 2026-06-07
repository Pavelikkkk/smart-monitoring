import { useEffect, useState } from "react";

import { getAnomalies } from "../services/api";

export default function Anomalies() {
  const [anomalies, setAnomalies] =
    useState<any[]>([]);

  const [search, setSearch] =
    useState("");

  const [visibleCount, setVisibleCount] =
    useState(12);

  useEffect(() => {
    getAnomalies()
      .then(setAnomalies)
      .catch(console.error);
  }, []);

  const filteredAnomalies =
    anomalies.filter((anomaly) =>
      `${anomaly.room} ${anomaly.type}`
        .toLowerCase()
        .includes(
          search.toLowerCase()
        )
    );

  const criticalCount =
    anomalies.filter(
      (a) =>
        a.severity ===
        "CRITICAL"
    ).length;

  const mlCount =
    anomalies.filter(
      (a) =>
        String(a.type)
          .toLowerCase()
          .includes("ml")
    ).length;

  function severityColor(
    severity: string
  ) {
    switch (severity) {
      case "CRITICAL":
        return "text-rose-400";

      case "WARNING":
        return "text-yellow-400";

      default:
        return "text-cyan-400";
    }
  }

  return (
    <div className="space-y-8">

      {/* HEADER */}

      <div>

        <h1 className="text-5xl font-bold mb-3">
          Anomalies
        </h1>

        <p className="text-slate-500">
          Monitor rule-based and
          machine learning alerts.
        </p>

      </div>

      {/* STATS */}

      <div
        className="
          grid
          grid-cols-1
          md:grid-cols-3
          gap-4
        "
      >

        <div
          className="
            bg-[#111827]
            rounded-2xl
            p-6
          "
        >
          <p className="text-slate-500">
            ⚠️ Total Alerts
          </p>

          <h2 className="text-3xl font-bold">
            {anomalies.length}
          </h2>
        </div>

        <div
          className="
            bg-[#111827]
            rounded-2xl
            p-6
          "
        >
          <p className="text-slate-500">
            🤖 ML Alerts
          </p>

          <h2 className="text-3xl font-bold text-cyan-400">
            {mlCount}
          </h2>
        </div>

        <div
          className="
            bg-[#111827]
            rounded-2xl
            p-6
          "
        >
          <p className="text-slate-500">
            🔥 Critical
          </p>

          <h2 className="text-3xl font-bold text-rose-400">
            {criticalCount}
          </h2>
        </div>

      </div>

      {/* SEARCH */}

      <input
        type="text"
        placeholder="Search anomalies..."
        value={search}
        onChange={(e) =>
          setSearch(
            e.target.value
          )
        }
        className="
          w-full
          bg-[#111827]
          border
          border-cyan-900/40
          rounded-2xl
          p-5
          outline-none
          focus:border-orange-300
        "
      />

      {/* CARDS */}

      <div
        className="
          grid
          grid-cols-1
          md:grid-cols-2
          xl:grid-cols-3
          gap-5
        "
      >

        {filteredAnomalies
          .slice(
            0,
            visibleCount
          )
          .map(
            (
              anomaly,
              index
            ) => (
              <div
                key={`${anomaly.room}-${anomaly.type}-${index}`}
                className="
                  bg-[#111827]
                  border
                  border-cyan-900/40
                  rounded-2xl
                  p-6
                  hover:border-cyan-400
                  transition
                "
              >

                <div className="flex justify-between mb-4">

                  <h2
                    className="
                      text-xl
                      font-bold
                    "
                  >
                    {anomaly.type}
                  </h2>

                  <span
                    className={
                      severityColor(
                        anomaly.severity
                      )
                    }
                  >
                    ●
                  </span>

                </div>

                <div className="space-y-3">

                  <div>
                    <span className="text-slate-500">
                      Room:
                    </span>{" "}
                    {anomaly.room}
                  </div>

                  <div>
                    <span className="text-slate-500">
                      Severity:
                    </span>{" "}

                    <span
                      className={
                        severityColor(
                          anomaly.severity
                        )
                      }
                    >
                      {
                        anomaly.severity
                      }
                    </span>
                  </div>

                  <div>
                    <span className="text-slate-500">
                      Score:
                    </span>{" "}
                    {Number(
                      anomaly.score
                    ).toFixed(3)}
                  </div>

                  <div className="text-slate-500 text-sm">

                    {new Date(
                      anomaly.detectedAt
                    ).toLocaleString()}

                  </div>

                </div>

              </div>
            )
          )}

      </div>

      {/* SHOW MORE */}

      {visibleCount <
        filteredAnomalies.length && (
          <div className="flex justify-center">

          <button
            onClick={() =>
              setVisibleCount(
                visibleCount + 12
              )
            }
            className="
              px-8
              py-4
              rounded-2xl
              text-cyan-400
              text-slate-900
              font-semibold
              hover:bg-cyan-400
              transition
            "
          >
            Show More
          </button>

          </div>
        )}

    </div>
  );
}