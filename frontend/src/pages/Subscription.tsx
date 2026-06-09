import { Link } from "react-router-dom";
import { useEffect, useState } from "react";

import {
  type Account,
  getAccount,
} from "../services/api";

export default function Subscription() {
  const [account, setAccount] =
    useState<Account | null>(null);

  const [error, setError] =
    useState("");

  useEffect(() => {
    getAccount()
      .then(setAccount)
      .catch((err) =>
        setError(
          err instanceof Error
            ? err.message
            : "Failed to load subscription"
        )
      );
  }, []);

  if (error) {
    return <div className="text-red-400">{error}</div>;
  }

  if (!account) {
    return <div className="text-slate-300">Loading subscription...</div>;
  }

  const isBusiness =
    account.accountType === "BUSINESS";

  return (
    <div className="space-y-8">
      <h1 className="text-5xl font-bold">
        Subscription
      </h1>

      <div className="bg-[#182234] border border-cyan-700/40 rounded-3xl p-5">
        <div className="grid grid-cols-1 md:grid-cols-3 gap-5">
          <div>
            <div className="text-slate-300 mb-2">
              Current Plan
            </div>
            <div className="text-4xl font-bold">
              {account.subscription.plan}
            </div>
          </div>

          <div>
            <div className="text-slate-300 mb-2">
              Account Type
            </div>
            <div className="text-xl">
              {account.accountType}
            </div>
          </div>

          <div>
            <div className="text-slate-300 mb-2">
              Status
            </div>
            <div className="text-green-400 text-xl">
              {account.subscription.status}
            </div>
          </div>
        </div>
      </div>

      <div className="grid grid-cols-1 md:grid-cols-3 gap-5">
        {isBusiness && (
          <div className="bg-[#182234] border border-cyan-700/40 rounded-3xl p-5">
            <div className="text-slate-300">
              Buildings
            </div>
            <div className="text-3xl font-bold mt-2">
              {account.subscription.maxBuildings}
            </div>
          </div>
        )}

        <div className="bg-[#182234] border border-cyan-700/40 rounded-3xl p-5">
          <div className="text-slate-300">
            Rooms
          </div>
          <div className="text-3xl font-bold mt-2">
            {account.subscription.maxRooms}
          </div>
        </div>

        <div className="bg-[#182234] border border-cyan-700/40 rounded-3xl p-5">
          <div className="text-slate-300">
            Devices
          </div>
          <div className="text-3xl font-bold mt-2">
            {account.subscription.maxDevices}
          </div>
        </div>
      </div>

      <div className="bg-[#182234] border border-cyan-700/40 rounded-3xl p-5">
        <h2 className="text-2xl font-bold mb-6">
          Included Features
        </h2>

        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
          <div>Rooms</div>
          <div>Devices</div>
          <div>Anomaly Detection</div>
          <div>Notifications</div>
          {isBusiness && <div>Buildings</div>}
          {isBusiness && <div>Organization Dashboard</div>}
        </div>

        <Link
          to="/upgrade"
          className="inline-block mt-8 px-8 py-3 rounded-2xl bg-cyan-500 text-slate-900 font-semibold hover:bg-cyan-400 transition"
        >
          View Available Plans
        </Link>
      </div>
    </div>
  );
}
