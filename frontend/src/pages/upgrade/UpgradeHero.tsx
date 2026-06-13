import type { Account } from "../../services/api";

export function UpgradeHero({
  account,
  isBusiness,
  currentPlan,
}: {
  account: Account | null;
  isBusiness: boolean;
  currentPlan: string;
}) {
  return (
    <div
      className="bg-[#111827]
      border
      border-cyan-700/40
      rounded-3xl
      p-10"
    >
      <div
        className="text-7xl
        mb-6"
      >
        {isBusiness ? "рџЏў" : "рџЏ "}
      </div>

      <h1
        className="text-5xl
        font-bold
        mb-4"
      >
        {isBusiness ? "Business Subscription" : "Personal Subscription"}
      </h1>

      <p
        className="text-xl
        text-slate-300"
      >
        Account Type:{" "}
        <span
          className="text-cyan-400
          font-semibold"
        >
          {account?.accountType ?? "PERSONAL"}
        </span>
      </p>

      <p
        className="text-lg
        text-slate-300
        mt-2"
      >
        Current Plan: {currentPlan}
      </p>
    </div>
  );
}
