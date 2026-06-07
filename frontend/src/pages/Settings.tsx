export default function Settings() {
  return (
    <>
      <h1 className="text-5xl font-bold mb-8">
        Settings
      </h1>

      <div className="grid gap-4">

        <div className="bg-[#111827] rounded-2xl p-6">
          <h2 className="text-xl font-bold mb-4">
            Detection
          </h2>

          <p>Power Threshold: 150 kW</p>
        </div>

        <div className="bg-[#111827] rounded-2xl p-6">
          <h2 className="text-xl font-bold mb-4">
            Machine Learning
          </h2>

          <p>ML Threshold: 0.80</p>
        </div>

        <div className="bg-[#111827] rounded-2xl p-6">
          <h2 className="text-xl font-bold mb-4">
            Telegram
          </h2>

          <p className="text-emerald-400">
            Connected
          </p>
        </div>

      </div>
    </>
  );
}