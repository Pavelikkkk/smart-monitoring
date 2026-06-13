const links = [
  {
    label: "GitHub",
    href: "https://github.com/Pavelikkkk/dorm-energy-monitoring",
  },
  {
    label: "Telegram",
    href: "https://t.me/atcocoon",
  },
];

export function HomeFooter() {
  return (
    <footer
      id="about"
      className="border-t
      border-cyan-700/40
      mt-24
      py-16"
    >
      <div
        className="grid
        grid-cols-1
        md:grid-cols-3
        gap-12"
      >
        <div>
          <div
            className="text-4xl
            mb-4"
          >
            вљЎ
          </div>

          <h3
            className="text-2xl
            font-bold
            mb-3"
          >
            Monitoring Platform
          </h3>

          <p className="text-slate-300">
            Smart IoT platform for energy monitoring, anomaly detection and
            analytics.
          </p>
        </div>

        <div>
          <h4
            className="text-xl
            font-semibold
            mb-4"
          >
            Links
          </h4>

          <div
            className="space-y-3
            text-slate-300"
          >
            {links.map((link) => (
              <a
                key={link.href}
                href={link.href}
                target="_blank"
                rel="noreferrer"
                className="block
                hover:text-cyan-400"
              >
                {link.label}
              </a>
            ))}
          </div>
        </div>
      </div>

      <div
        className="mt-12
        pt-8
        border-t
        border-slate-800
        text-slate-300
        text-center"
      >
        Monitoring Platform вЂў Version 2.0
      </div>
    </footer>
  );
}
