const audiences = [
  {
    icon: "рџЏ ",
    title: "Personal Users",
    description:
      "Monitor your rooms, connected devices and receive instant anomaly notifications.",
  },
  {
    icon: "рџЏў",
    title: "Organizations",
    description:
      "Manage buildings, users, energy analytics and AI anomaly detection.",
  },
];

export function HomeAudience() {
  return (
    <section className="py-24">
      <h2
        className="text-5xl
        font-bold
        text-center
        mb-16"
      >
        Who Is It For?
      </h2>

      <div
        className="grid
        grid-cols-1
        md:grid-cols-2
        gap-5"
      >
        {audiences.map((audience) => (
          <div
            key={audience.title}
            className="bg-[#111827]
            rounded-3xl
            p-10
            border
            border-cyan-700/40"
          >
            <div
              className="text-6xl
              mb-6"
            >
              {audience.icon}
            </div>

            <h3
              className="text-3xl
              font-bold
              mb-4"
            >
              {audience.title}
            </h3>

            <p className="text-slate-300">{audience.description}</p>
          </div>
        ))}
      </div>
    </section>
  );
}
