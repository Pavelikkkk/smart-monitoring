import { useEffect, useState } from "react";
import {
  type Account,
  getAccount,
} from "../services/api";

export default function Upgrade() {
  const [account, setAccount] =
    useState<Account | null>(null);

  useEffect(() => {
    getAccount()
      .then(setAccount)
      .catch(() => setAccount(null));
  }, []);

  const isBusiness =
    account?.accountType ===
    "BUSINESS";

  const currentPlan =
    account?.subscription.plan ?? "STANDARD";

  return (
    <div
      className="
        max-w-7xl
        mx-auto
        space-y-10
      "
    >
      {/* HERO */}

      <div
        className="
          bg-[#111827]
          border
          border-cyan-700/40
          rounded-3xl
          p-10
        "
      >
        <div className="text-7xl mb-6">
          {isBusiness ? "🏢" : "🏠"}
        </div>

        <h1
          className="
            text-5xl
            font-bold
            mb-4
          "
        >
          {isBusiness
            ? "Business Subscription"
            : "Personal Subscription"}
        </h1>

        <p
          className="
            text-xl
            text-slate-300
          "
        >
          Account Type:
          {" "}
          <span className="text-cyan-400 font-semibold">
            {account?.accountType ?? "PERSONAL"}
          </span>
        </p>

        <p
          className="
            text-lg
            text-slate-300
            mt-2
          "
        >
          Current Plan:
          {" "}
          {currentPlan}
        </p>
      </div>

      {/* PERSONAL PLANS */}

      {!isBusiness && (
        <div>

          <h2
            className="
              text-4xl
              font-bold
              mb-6
            "
          >
            👤 Personal Plans
          </h2>

          <div
            className="
              grid
              grid-cols-1
              lg:grid-cols-3
              gap-5
            "
          >

            {/* Free */}

            <div
              className="
                bg-[#111827]
                rounded-3xl
                border
                border-cyan-500
                p-5
              "
            >
              <div
                className="
                  inline-block
                  px-4
                  py-2
                  rounded-xl
                  bg-cyan-500
                  text-slate-900
                  font-semibold
                  mb-4
                "
              >
                Current
              </div>

              <h3 className="text-3xl font-bold">
                Free
              </h3>

              <div className="text-slate-300 mt-2">
                Free
              </div>

              <div className="space-y-3 mt-8">

                <div>✅ 2 Rooms</div>

                <div>✅ 5 Devices</div>

                <div>✅ Anomalies</div>

                <div>❌ Analytics</div>

              </div>

            </div>

            {/* BASIC */}

            <div
              className="
                bg-[#111827]
                rounded-3xl
                border
                border-cyan-700/40
                p-5
              "
            >
              <h3 className="text-3xl font-bold">
                Basic
              </h3>

              <div className="text-cyan-400 mt-2">
                $4.99 / month
              </div>

              <div className="space-y-3 mt-8">

                <div>✅ 10 Rooms</div>

                <div>✅ 30 Devices</div>

                <div>✅ Notifications</div>

                <div>✅ Full History</div>

              </div>

            </div>

            {/* PRO */}

            <div
              className="
                bg-[#111827]
                rounded-3xl
                border
                border-orange-400
                p-5
              "
            >
              <div
                className="
                  inline-block
                  px-4
                  py-2
                  rounded-xl
                  bg-orange-400
                  text-slate-900
                  font-semibold
                  mb-4
                "
              >
                Popular
              </div>

              <h3 className="text-3xl font-bold">
                Pro
              </h3>

              <div className="text-cyan-400 mt-2">
                $9.99 / month
              </div>

              <div className="space-y-3 mt-8">

                <div>✅ 50 Rooms</div>

                <div>✅ 100 Devices</div>

                <div>✅ AI Analytics</div>

                <div>✅ Priority Support</div>

              </div>

            </div>

          </div>

        </div>
      )}

      {/* BUSINESS PLANS */}

      {isBusiness && (
        <div>

          <h2
            className="
        text-4xl
        font-bold
        mb-6
      "
          >
            🏢 Business Plans
          </h2>

          <div
            className="
        grid
        grid-cols-1
        lg:grid-cols-3
        gap-5
      "
          >

            {/* Free */}

            <div
              className={`
          bg-[#111827]
          rounded-3xl
          border
          p-5
          ${currentPlan === "FREE"
                  ? "border-cyan-500"
                  : "border-cyan-700/40"
                }
        `}
            >
              {currentPlan === "FREE" && (
                <div
                  className="
              inline-block
              px-4
              py-2
              rounded-xl
              bg-cyan-500
              text-slate-900
              font-semibold
              mb-4
            "
                >
                  Current
                </div>
              )}

              <h3 className="text-3xl font-bold">
                Free
              </h3>

              <div className="text-slate-300 mt-2">
                Free
              </div>

              <div className="space-y-3 mt-8">

                <div>✅ 1 Building</div>

                <div>✅ 5 Rooms</div>

                <div>✅ 10 Devices</div>

                <div>❌ Advanced Analytics</div>

              </div>

            </div>

            {/* STANDARD */}

            <div
              className={`
          bg-[#111827]
          rounded-3xl
          border
          p-5
          ${currentPlan === "STANDARD"
                  ? "border-cyan-500"
                  : "border-cyan-700/40"
                }
        `}
            >
              <div className="flex items-center gap-3 mb-4">

                <div
                  className="
              inline-block
              px-4
              py-2
              rounded-xl
              bg-orange-400
              text-slate-900
              font-semibold
            "
                >
                  Best Choice
                </div>

                {currentPlan === "STANDARD" && (
                  <div
                    className="
                inline-block
                px-4
                py-2
                rounded-xl
                bg-cyan-500
                text-slate-900
                font-semibold
              "
                  >
                    Current
                  </div>
                )}

              </div>

              <h3 className="text-3xl font-bold">
                Standard
              </h3>

              <div className="text-cyan-400 mt-2">
                $29.99 / month
              </div>

              <div className="space-y-3 mt-8">

                <div>✅ 5 Buildings</div>

                <div>✅ 100 Rooms</div>

                <div>✅ 500 Devices</div>

                <div>✅ Analytics</div>

              </div>

            </div>

            {/* ENTERPRISE */}

            <div
              className={`
          bg-[#111827]
          rounded-3xl
          border
          p-5
          ${currentPlan === "ENTERPRISE"
                  ? "border-cyan-500"
                  : "border-cyan-700/40"
                }
        `}
            >
              {currentPlan === "ENTERPRISE" && (
                <div
                  className="
              inline-block
              px-4
              py-2
              rounded-xl
              bg-cyan-500
              text-slate-900
              font-semibold
              mb-4
            "
                >
                  Current
                </div>
              )}

              <h3 className="text-3xl font-bold">
                Enterprise
              </h3>

              <div className="text-cyan-400 mt-2">
                Contact Sales
              </div>

              <div className="space-y-3 mt-8">

                <div>✅ Unlimited Buildings</div>

                <div>✅ Unlimited Rooms</div>

                <div>✅ Unlimited Devices</div>

                <div>✅ Team Management</div>

                <div>✅ Reports</div>

                <div>✅ Dedicated Support</div>

              </div>

            </div>

          </div>

        </div>
      )}

      {/* CTA */}

      <div
        className="
          bg-[#111827]
          rounded-3xl
          border
          border-cyan-700/40
          p-10
          text-center
        "
      >
        <h2
          className="
            text-4xl
            font-bold
            mb-4
          "
        >
          Ready to upgrade?
        </h2>

        <p
          className="
            text-slate-300
            mb-8
          "
        >
          Unlock more devices, rooms,
          buildings and analytics.
        </p>

        <button
          className="
            px-10
            py-4
            rounded-2xl
            bg-cyan-500
            text-slate-900
            font-bold
            text-lg
            hover:bg-cyan-400
            transition
          "
        >
          Upgrade Subscription
        </button>

      </div>

    </div>
  );
}
