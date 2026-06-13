import { Link } from "react-router-dom";

export default function Home() {
  return (
    <div className="w-full">
      {/* HERO */}

      <section
        className="min-h-[70vh]
              flex
              flex-col
              justify-center
              items-center
              text-center"
      >
        <div className="text-8xl
              mb-8">⚡</div>

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

        <div className="flex
              gap-4
              flex-wrap">
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
          <div
            className="bg-[#111827]
                  rounded-3xl
                  p-5
                  border
                  border-cyan-700/40"
          >
            <div className="text-5xl
                  font-bold
                  text-cyan-400">1200+</div>

            <div className="text-slate-300
                  mt-2">Connected Devices</div>
          </div>

          <div
            className="bg-[#111827]
                  rounded-3xl
                  p-5
                  border
                  border-cyan-700/40"
          >
            <div className="text-5xl
                  font-bold
                  text-cyan-400">25+</div>

            <div className="text-slate-300
                  mt-2">Buildings</div>
          </div>

          <div
            className="bg-[#111827]
                  rounded-3xl
                  p-5
                  border
                  border-cyan-700/40"
          >
            <div className="text-5xl
                  font-bold
                  text-cyan-400">300+</div>

            <div className="text-slate-300
                  mt-2">Active Users</div>
          </div>

          <div
            className="bg-[#111827]
                  rounded-3xl
                  p-5
                  border
                  border-cyan-700/40"
          >
            <div className="text-5xl
                  font-bold
                  text-cyan-400">99.9%</div>

            <div className="text-slate-300
                  mt-2">Uptime</div>
          </div>
        </div>
      </section>

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
          <div
            className="bg-[#111827]
                  rounded-3xl
                  p-10
                  border
                  border-cyan-700/40"
          >
            <div className="text-6xl
                  mb-6">🏠</div>

            <h3 className="text-3xl
                  font-bold
                  mb-4">Personal Users</h3>

            <p className="text-slate-300">
              Monitor your rooms, connected devices and receive instant anomaly
              notifications.
            </p>
          </div>

          <div
            className="bg-[#111827]
                  rounded-3xl
                  p-10
                  border
                  border-cyan-700/40"
          >
            <div className="text-6xl
                  mb-6">🏢</div>

            <h3 className="text-3xl
                  font-bold
                  mb-4">Organizations</h3>

            <p className="text-slate-300">
              Manage buildings, users, energy analytics and AI anomaly
              detection.
            </p>
          </div>
        </div>
      </section>

      {/* FEATURE 1 */}

      <section
        id="features"
        className="grid
              grid-cols-1
              xl:grid-cols-2
              gap-12
              py-24
              items-center"
      >
        <div>
          <h2 className="text-5xl
                font-bold
                mb-6">Real-Time Monitoring</h2>

          <p className="text-xl
                text-slate-300">
            ESP32 devices stream live power, motion and light measurements
            through MQTT.
          </p>
        </div>

        <div
          className="h-96
                bg-[#111827]
                rounded-3xl
                border
                border-cyan-700/40
                flex
                items-center
                justify-center"
        >
          <img
            src="https://images.unsplash.com/photo-1518770660439-4636190af475"
            alt="IoT Monitoring"
            className="w-full
                  h-96
                  object-cover
                  rounded-3xl"
          />
        </div>
      </section>

      {/* FEATURE 2 */}

      <section
        className="grid
              grid-cols-1
              xl:grid-cols-2
              gap-12
              py-24
              items-center"
      >
        <div
          className="order-2
                xl:order-1
                h-96
                bg-[#111827]
                rounded-3xl
                border
                border-cyan-700/40
                flex
                items-center
                justify-center"
        >
          <img
            src="https://avatars.mds.yandex.net/i?id=9cb64b10b0a2964b06d946de60c6e61a_l-12651048-images-thumbs&n=13"
            alt="Smart Building"
            className="w-full
                  h-96
                  object-cover
                  rounded-3xl"
          />
        </div>

        <div className="order-1
              xl:order-2">
          <h2 className="text-5xl
                font-bold
                mb-6">Hybrid Detection</h2>

          <p className="text-xl
                text-slate-300">
            Rule-based and ONNX-powered machine learning anomaly detection.
          </p>
        </div>
      </section>

      {/* FEATURE 3 */}

      <section
        className="grid
              grid-cols-1
              xl:grid-cols-2
              gap-12
              py-24
              items-center"
      >
        <div>
          <h2 className="text-5xl
                font-bold
                mb-6">Analytics Platform</h2>

          <p className="text-xl
                text-slate-300">
            Historical trends, room statistics, anomaly reports and energy
            insights.
          </p>
        </div>

        <div
          className="h-96
                bg-[#111827]
                rounded-3xl
                border
                border-cyan-700/40
                flex
                items-center
                justify-center"
        >
          <img
            src="https://images.unsplash.com/photo-1460925895917-afdab827c52f"
            alt="Analytics Dashboard"
            className="w-full
                  h-96
                  object-cover
                  rounded-3xl"
          />
        </div>
      </section>

      {/* PROJECT OVERVIEW */}

      <section className="py-24">
        <h2 className="text-5xl
              font-bold
              text-center
              mb-5">
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

        <div className="grid
              grid-cols-1
              lg:grid-cols-3
              gap-6">
          {/* SUPPORTED MONITORING */}

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
            <div className="text-6xl
                  mb-6">⚡</div>

            <h3 className="text-4xl
                  font-bold
                  mb-3">Supported Monitoring</h3>

            <p
              className="text-slate-400
                    text-lg
                    mb-8"
            >
              Track room activity and energy consumption in real time.
            </p>

            <div
              className="h-px
                    bg-slate-700
                    mb-8"
            />

            <div className="space-y-5
                  text-xl">
              <div>Energy Consumption</div>

              <div>Motion Detection</div>

              <div>Lighting Activity</div>

              <div>Device Connectivity</div>

              <div>Room Activity</div>

              <div>Usage Trends</div>
            </div>
          </div>

          {/* ANOMALIES */}

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
            <div className="text-6xl
                  mb-6">🔍</div>

            <h3 className="text-4xl
                  font-bold
                  mb-3">Detect Anomalies</h3>

            <p
              className="text-slate-400
                    text-lg
                    mb-8"
            >
              Identify unusual behavior before it becomes an expensive problem.
            </p>

            <div
              className="h-px
                    bg-slate-700
                    mb-8"
            />

            <div className="space-y-5
                  text-xl">
              <div>Lights Left On</div>

              <div>Power Without Motion</div>

              <div>Abnormal Consumption</div>

              <div>Machine Learning Detection</div>

              <div>Real-Time Alerts</div>

              <div>Severity Classification</div>
            </div>
          </div>

          {/* NOTIFICATIONS */}

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
            <div className="text-6xl
                  mb-6">📨</div>

            <h3 className="text-4xl
                  font-bold
                  mb-3">Instant Notifications</h3>

            <p
              className="text-slate-400
                    text-lg
                    mb-8"
            >
              Stay informed with alerts and analytics wherever you are.
            </p>

            <div
              className="h-px
                    bg-slate-700
                    mb-8"
            />

            <div className="space-y-5
                  text-xl">
              <div>Telegram Alerts</div>

              <div>Active Incident Tracking</div>

              <div>Device Status Updates</div>

              <div>Historical Records</div>

              <div>Analytics Dashboard</div>

              <div>Building Monitoring</div>
            </div>
          </div>
        </div>
      </section>

      {/* FOOTER */}

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
            <div className="text-4xl
                  mb-4">⚡</div>

            <h3 className="text-2xl
                  font-bold
                  mb-3">Monitoring Platform</h3>

            <p className="text-slate-300">
              Smart IoT platform for energy monitoring, anomaly detection and
              analytics.
            </p>
          </div>

          <div>
            <h4 className="text-xl
                  font-semibold
                  mb-4">Links</h4>

            <div className="space-y-3
                  text-slate-300">
              <a
                href="https://github.com/Pavelikkkk/dorm-energy-monitoring"
                target="_blank"
                rel="noreferrer"
                className="block
                      hover:text-cyan-400"
              >
                GitHub
              </a>

              <a
                href="https://t.me/atcocoon"
                target="_blank"
                rel="noreferrer"
                className="block
                      hover:text-cyan-400"
              >
                Telegram
              </a>
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
          Monitoring Platform • Version 2.0
        </div>
      </footer>
    </div>
  );
}
