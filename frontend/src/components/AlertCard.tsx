type Props = {
  room: string;
  type: string;
  severity: "INFO" | "WARNING" | "CRITICAL";
  score?: number;

  status?: "ACTIVE" | "ACKNOWLEDGED" | "RESOLVED";
};

export default function AlertCard({
  room,
  type,
  severity,
  score,
  status = "ACTIVE",
}: Props) {
  const severityColor = {
    INFO: "text-cyan-400",
    WARNING: "text-yellow-400",
    CRITICAL: "text-rose-400",
  };

  const statusColor = {
    ACTIVE: "bg-red-500",
    ACKNOWLEDGED: "bg-yellow-500",
    RESOLVED: "bg-green-500",
  };

  return (
    <div className="bg-[#111827]
          border
          border-cyan-700/40
          rounded-2xl
          p-5">
      <div className="flex
            justify-between
            items-start
            mb-4">
        <div>
          <h3 className="text-xl
                font-bold">⚠ {room}</h3>

          <p className="text-slate-300
                mt-1">{type}</p>
        </div>

        <div className="text-right">
          <div
            className={`
              ${severityColor[severity]}
              font-semibold
            `}
          >
            {severity}
          </div>

          <div
            className="flex
                  items-center
                  justify-end
                  gap-2
                  mt-2"
          >
            <div
              className={`
                w-2
                h-2
                rounded-full
                ${statusColor[status]}
              `}
            />

            <span className="text-xs
                  text-slate-300">{status}</span>
          </div>
        </div>
      </div>

      {score !== undefined && (
        <div className="mt-2">
          <p className="text-slate-300
                text-sm">ML Score</p>

          <p className="text-cyan-400
                font-bold">{score.toFixed(2)}</p>
        </div>
      )}
    </div>
  );
}
