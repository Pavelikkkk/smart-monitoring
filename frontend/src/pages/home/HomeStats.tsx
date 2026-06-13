const stats = [
  ["1200+", "Connected Devices"],
  ["25+", "Buildings"],
  ["300+", "Active Users"],
  ["99.9%", "Uptime"],
];

function StatTile({ value, label }: { value: string; label: string }) {
  return (
    <div
      className="bg-[#111827]
      rounded-3xl
      p-5
      border
      border-cyan-700/40"
    >
      <div
        className="text-5xl
        font-bold
        text-cyan-400"
      >
        {value}
      </div>

      <div
        className="text-slate-300
        mt-2"
      >
        {label}
      </div>
    </div>
  );
}

export function HomeStats() {
  return (
    <section
      id="stats"
      className="py-24"
    >
      <div
        className="grid
        grid-cols-2
        lg:grid-cols-4
        gap-5"
      >
        {stats.map(([value, label]) => (
          <StatTile
            key={label}
            value={value}
            label={label}
          />
        ))}
      </div>
    </section>
  );
}
