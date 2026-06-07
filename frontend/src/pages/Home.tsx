export default function Home() {
  return (
    <div className="space-y-16">

      {/* HERO */}

      <section className="text-center py-16">

        <div className="mb-6 text-7xl">
          ⚡
        </div>

        <h1 className="text-6xl font-bold mb-6">
          Dorm Energy
        </h1>

        <p className="text-xl text-slate-400 max-w-3xl mx-auto leading-relaxed">
          Smart IoT platform for real-time dormitory energy
          monitoring, anomaly detection and consumption analytics.
        </p>

        <div className="flex flex-wrap justify-center gap-4 mt-10">

          <a
            href="/analytics"
            className="
              px-8
              py-4
              rounded-xl
              bg-cyan-500
              text-white
              font-semibold
              hover:bg-cyan-600
              transition
            "
          >
            Open Dashboard
          </a>

          <button
            disabled
            className="
              px-8
              py-4
              rounded-xl
              border
              border-slate-600
              text-slate-400
              cursor-not-allowed
            "
          >
            Desktop App (Coming Soon)
          </button>

        </div>

      </section>

      {/* FEATURES */}

      <section>

        <h2 className="text-3xl font-bold mb-8 text-center">
          Platform Features
        </h2>

        <div className="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-4 gap-6">

          <div className="bg-slate-800 border border-slate-700 rounded-xl p-6">
            <h3 className="text-xl font-semibold mb-3">
              📡 Real-Time Monitoring
            </h3>

            <p className="text-slate-400">
              MQTT-based sensor stream processing
              with room state aggregation.
            </p>
          </div>

          <div className="bg-slate-800 border border-slate-700 rounded-xl p-6">
            <h3 className="text-xl font-semibold mb-3">
              🤖 ML Detection
            </h3>

            <p className="text-slate-400">
              Hybrid anomaly detection using
              rules and ONNX Runtime models.
            </p>
          </div>

          <div className="bg-slate-800 border border-slate-700 rounded-xl p-6">
            <h3 className="text-xl font-semibold mb-3">
              📊 Analytics
            </h3>

            <p className="text-slate-400">
              Historical consumption tracking
              and advanced visualization.
            </p>
          </div>

          <div className="bg-slate-800 border border-slate-700 rounded-xl p-6">
            <h3 className="text-xl font-semibold mb-3">
              🔔 Alerts
            </h3>

            <p className="text-slate-400">
              Telegram notifications for
              critical incidents.
            </p>
          </div>

        </div>

      </section>

      {/* ARCHITECTURE */}

      <section>

        <h2 className="text-3xl font-bold mb-8 text-center">
          System Architecture
        </h2>

        <div className="bg-slate-800 border border-slate-700 rounded-xl p-8">

          <div className="flex flex-col xl:flex-row items-center justify-between gap-6 text-center">

            <div>
              <div className="text-4xl mb-2">
                📡
              </div>

              <div className="font-semibold">
                ESP32 Sensors
              </div>
            </div>

            <div className="text-cyan-400 text-3xl">
              →
            </div>

            <div>
              <div className="text-4xl mb-2">
                📨
              </div>

              <div className="font-semibold">
                MQTT Broker
              </div>
            </div>

            <div className="text-cyan-400 text-3xl">
              →
            </div>

            <div>
              <div className="text-4xl mb-2">
                ⚙️
              </div>

              <div className="font-semibold">
                C++ Backend
              </div>
            </div>

            <div className="text-cyan-400 text-3xl">
              →
            </div>

            <div>
              <div className="text-4xl mb-2">
                🗄️
              </div>

              <div className="font-semibold">
                TimescaleDB
              </div>
            </div>

            <div className="text-cyan-400 text-3xl">
              →
            </div>

            <div>
              <div className="text-4xl mb-2">
                📊
              </div>

              <div className="font-semibold">
                Dashboard
              </div>
            </div>

          </div>

        </div>

      </section>

      {/* TECHNOLOGY STACK */}

      <section>

        <h2 className="text-3xl font-bold mb-8 text-center">
          Technology Stack
        </h2>

        <div className="grid grid-cols-2 md:grid-cols-3 xl:grid-cols-6 gap-4">

          {[
            "C++20",
            "Drogon",
            "MQTT",
            "TimescaleDB",
            "React",
            "ONNX",
          ].map((tech) => (
            <div
              key={tech}
              className="
                bg-slate-800
                border
                border-slate-700
                rounded-xl
                p-4
                text-center
                font-semibold
              "
            >
              {tech}
            </div>
          ))}

        </div>

      </section>

    </div>
  );
}