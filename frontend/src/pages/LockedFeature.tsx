// pages/LockedFeature.tsx

export default function LockedFeature() {
  return (
    <div
      className="max-w-4xl
            mx-auto
            mt-10"
    >
      <div
        className="bg-[#111827]
              border
              border-cyan-700/40
              rounded-3xl
              p-10"
      >
        <div className="text-6xl
              mb-6">🔒</div>

        <h1
          className="text-4xl
                font-bold
                mb-4"
        >
          Business Feature
        </h1>

        <p
          className="text-slate-300
                text-lg
                mb-8"
        >
          This feature is available only for Business subscriptions.
        </p>

        <div className="space-y-4">
          <div>✅ Buildings Management</div>

          <div>✅ Organization Dashboard</div>

          <div>✅ Advanced Analytics</div>

          <div>✅ Team Management</div>
        </div>

        <button
          className="mt-10
                px-8
                py-4
                rounded-2xl
                bg-cyan-500
                text-slate-900
                font-semibold"
        >
          Upgrade to Business
        </button>
      </div>
    </div>
  );
}
