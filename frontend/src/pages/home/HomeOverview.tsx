const overviewCards = [
  {
    icon: "вљЎ",
    title: "Supported Monitoring",
    description: "Track room activity and energy consumption in real time.",
    items: [
      "Energy Consumption",
      "Motion Detection",
      "Lighting Activity",
      "Device Connectivity",
      "Room Activity",
      "Usage Trends",
    ],
  },
  {
    icon: "рџ”Ќ",
    title: "Detect Anomalies",
    description:
      "Identify unusual behavior before it becomes an expensive problem.",
    items: [
      "Lights Left On",
      "Power Without Motion",
      "Abnormal Consumption",
      "Machine Learning Detection",
      "Real-Time Alerts",
      "Severity Classification",
    ],
  },
  {
    icon: "рџ“Ё",
    title: "Instant Notifications",
    description: "Stay informed with alerts and analytics wherever you are.",
    items: [
      "Telegram Alerts",
      "Active Incident Tracking",
      "Device Status Updates",
      "Historical Records",
      "Analytics Dashboard",
      "Building Monitoring",
    ],
  },
];

function OverviewCard({ card }: { card: (typeof overviewCards)[number] }) {
  return (
    <div
      className="bg-[#111827]
      border
      border-cyan-700/40
      rounded-3xl
      p-10
      hover:border-cyan-400
      hover:-translate-y-1
      transition-all
      duration-300"
    >
      <div
        className="text-6xl
        mb-6"
      >
        {card.icon}
      </div>

      <h3
        className="text-4xl
        font-bold
        mb-3"
      >
        {card.title}
      </h3>

      <p
        className="text-slate-400
        text-lg
        mb-8"
      >
        {card.description}
      </p>

      <div
        className="h-px
        bg-slate-700
        mb-8"
      />

      <div
        className="space-y-5
        text-xl"
      >
        {card.items.map((item) => (
          <div key={item}>{item}</div>
        ))}
      </div>
    </div>
  );
}

export function HomeOverview() {
  return (
    <section className="py-24">
      <h2
        className="text-5xl
        font-bold
        text-center
        mb-5"
      >
        Smart Energy Monitoring
      </h2>

      <p
        className="text-center
        text-slate-400
        text-xl
        max-w-4xl
        mx-auto
        mb-16"
      >
        Monitor energy usage, detect abnormal behavior and receive instant
        alerts across your entire environment.
      </p>

      <div
        className="grid
        grid-cols-1
        lg:grid-cols-3
        gap-6"
      >
        {overviewCards.map((card) => (
          <OverviewCard
            key={card.title}
            card={card}
          />
        ))}
      </div>
    </section>
  );
}
