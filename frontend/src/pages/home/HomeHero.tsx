import { Link } from "react-router-dom";

export function HomeHero() {
  return (
    <section
      className="min-h-[70vh]
      flex
      flex-col
      justify-center
      items-center
      text-center"
    >
      <div
        className="text-8xl
        mb-8"
      >
        вљЎ
      </div>

      <h1
        className="text-5xl
        md:text-7xl
        font-bold
        max-w-5xl
        mb-8"
      >
        The Future of Smart Monitoring
      </h1>

      <p
        className="text-xl
        text-slate-300
        max-w-3xl
        mb-10"
      >
        Real-time monitoring, machine learning anomaly detection and analytics
        platform for dormitories and smart buildings.
      </p>

      <div
        className="flex
        gap-4
        flex-wrap"
      >
        <Link
          to="/register"
          className="px-8
          py-4
          rounded-2xl
          bg-cyan-500
          text-slate-900
          font-semibold"
        >
          Start now
        </Link>

        <button
          disabled
          className="px-8
          py-4
          rounded-2xl
          border
          border-slate-600
          text-slate-300"
        >
          API Reference(Coming soon)
        </button>
      </div>
    </section>
  );
}
