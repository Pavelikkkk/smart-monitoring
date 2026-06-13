export function UpgradeCta() {
  return (
    <div
      className="bg-[#111827]
      rounded-3xl
      border
      border-cyan-700/40
      p-10
      text-center"
    >
      <h2
        className="text-4xl
        font-bold
        mb-4"
      >
        Ready to upgrade?
      </h2>

      <p
        className="text-slate-300
        mb-8"
      >
        Unlock more devices, rooms, buildings and analytics.
      </p>

      <button
        className="px-10
        py-4
        rounded-2xl
        bg-cyan-500
        text-slate-900
        font-bold
        text-lg
        hover:bg-cyan-400
        transition"
      >
        Upgrade Subscription
      </button>
    </div>
  );
}
