import { Link } from "react-router-dom";
import Sidebar from "./Sidebar";
import { useEffect, useState } from "react";
import { clearToken, type Account, getAccount } from "../services/api";

export default function Header() {
  const [profileOpen, setProfileOpen] = useState(false);

  const [account, setAccount] = useState<Account | null>(null);

  useEffect(() => {
    getAccount()
      .then(setAccount)
      .catch(() => setAccount(null));
  }, []);

  const username = account?.username ?? "Guest";

  return (
    <header
      className="sticky
            top-0
            z-40
            bg-[#020617]
            border-b
            border-slate-800"
    >
      <div
        className="h-20
              px-8
              flex
              items-center"
      >
        {/* LEFT */}

        <div
          className="flex
                items-center
                gap-5"
        >
          <Sidebar />

          <Link
            to="/account"
            className="flex
                  items-center
                  gap-4"
          >
            <span className="text-4xl">⚡</span>
          </Link>
        </div>

        <div
          className="ml-auto
                relative
                flex
                items-center
                gap-4"
        >
          <span
            className="text-lg
                  text-slate-300"
          >
            {username}
          </span>

          <button
            onClick={() => setProfileOpen(!profileOpen)}
            className="w-11
                  h-11
                  rounded-full
                  bg-cyan-500
                  flex
                  items-center
                  justify-center
                  text-slate-900
                  font-bold
                  text-lg
                  hover:scale-105
                  transition"
          >
            {username.charAt(0).toUpperCase()}
          </button>

          {profileOpen && (
            <div
              className="absolute
                    right-0
                    top-14
                    w-72
                    bg-[#111827]
                    border
                    border-slate-800
                    rounded-2xl
                    shadow-2xl
                    overflow-hidden"
            >
              {/* USER */}

              <div
                className="p-5
                      border-b
                      border-slate-800"
              >
                <div
                  className="font-semibold
                        text-lg"
                >
                  {username}
                </div>

                <div
                  className="text-sm
                        text-slate-300"
                >
                  {account?.email ?? ""}
                </div>

                <div
                  className="mt-2
                        text-cyan-400
                        text-sm"
                >
                  {account?.subscription.plan ?? "STANDARD"} Plan
                </div>
              </div>

              {/* LINKS */}

              <div className="p-2">
                <Link
                  to="/account"
                  className="block
                        px-4
                        py-3
                        rounded-xl
                        hover:bg-slate-800"
                >
                  👤 Account
                </Link>

                <Link
                  to="/subscription"
                  className="block
                        px-4
                        py-3
                        rounded-xl
                        hover:bg-slate-800"
                >
                  💳 Subscription
                </Link>

                <Link
                  to="/settings"
                  className="block
                        px-4
                        py-3
                        rounded-xl
                        hover:bg-slate-800"
                >
                  ⚙️ Settings
                </Link>
              </div>

              {/* LOGOUT */}

              <div
                className="border-t
                      border-slate-800
                      p-2"
              >
                <button
                  onClick={() => {
                    clearToken();
                    window.location.hash = "#/login";
                  }}
                  className="w-full
                        text-left
                        px-4
                        py-3
                        rounded-xl
                        text-red-400
                        hover:bg-red-500/10"
                >
                  🚪 Sign Out
                </button>
              </div>
            </div>
          )}
        </div>
      </div>
    </header>
  );
}
