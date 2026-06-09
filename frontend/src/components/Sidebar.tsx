import { Link } from "react-router-dom";
import { useEffect, useState } from "react";

import {
    type Account,
    getAccount,
} from "../services/api";

export default function Sidebar() {
    const [open, setOpen] =
        useState(false);

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

    const isAdmin =
        account?.role === "ADMIN";

    return (
        <>
            {/* MENU BUTTON */}

            <button
                onClick={() => setOpen(true)}
                className="
          w-12
          h-12
          rounded-2xl
          bg-[#111827]
          border
          border-cyan-700/40
          hover:bg-cyan-500
          transition
        "
            >
                ☰
            </button>

            {/* SIDEBAR */}

            {open && (
                <div
                    className="
            fixed
            inset-0
            z-50
            bg-black/60
            backdrop-blur-sm
          "
                >
                    <aside
                        className="
              w-80
              h-screen
              bg-slate-950
              border-r
              border-slate-800
              p-5
              flex
              flex-col
            "
                    >
                        {/* HEADER */}

                        <div
                            className="
                flex
                justify-between
                items-center
                mb-10
              "
                        >
                            <Link
                                to="/account"
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="
                  flex
                  items-center
                  gap-3
                "
                            >
                                <span className="text-4xl">
                                    ⚡
                                </span>

                                <span
                                    className="
                    text-2xl
                    font-bold
                  "
                                >
                                    SMATI
                                </span>
                            </Link>

                            <button
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="
                  text-2xl
                  hover:text-cyan-400
                "
                            >
                                ✕
                            </button>
                        </div>

                        {/* NAVIGATION */}

                        <div
                            className="
                space-y-5
                text-lg
              "
                        >
                            <Link
                                to="/account"
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="
                  block
                  hover:text-cyan-400
                  transition
                "
                            >
                                👤 Account
                            </Link>

                            {/* BUSINESS ONLY */}

                            {isBusiness && (
                                <>
                                    <Link
                                        to="/buildings"
                                        onClick={() =>
                                            setOpen(false)
                                        }
                                        className="
                      block
                      hover:text-cyan-400
                      transition
                    "
                                    >
                                        🏢 Buildings
                                    </Link>

                                    <Link
                                        to="/analytics"
                                        onClick={() =>
                                            setOpen(false)
                                        }
                                        className="
                      block
                      hover:text-cyan-400
                      transition
                    "
                                    >
                                        📈 Analytics
                                    </Link>
                                </>
                            )}

                            {/* COMMON */}

                            <Link
                                to="/rooms"
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="
                  block
                  hover:text-cyan-400
                  transition
                "
                            >
                                🚪 Rooms
                            </Link>

                            <Link
                                to="/devices"
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="
                  block
                  hover:text-cyan-400
                  transition
                "
                            >
                                📡 Devices
                            </Link>

                            <Link
                                to="/anomalies"
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="
                  block
                  hover:text-cyan-400
                  transition
                "
                            >
                                ⚠️ Anomalies
                            </Link>

                            <Link
                                to="/settings"
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="
                  block
                  hover:text-cyan-400
                  transition
                "
                            >
                                ⚙️ Settings
                            </Link>

                            {isAdmin && (
                                <Link
                                    to="/admin"
                                    onClick={() =>
                                        setOpen(false)
                                    }
                                    className="
                  block
                  text-cyan-400
                  hover:text-cyan-300
                  transition
                "
                                >
                                    Admin
                                </Link>
                            )}

                            {/* UPGRADE */}

                            <Link
                                to="/upgrade"
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="
                  block
                  mt-8
                  text-cyan-400
                  font-semibold
                  hover:text-cyan-300
                  transition
                "
                            >
                                🚀 Upgrade
                            </Link>
                        </div>

                        {/* USER */}

                        <div
                            className="
                mt-auto
                pt-6
                border-t
                border-slate-800
              "
                        >
                            <div
                                className="
                  text-sm
                  text-slate-300
                  mb-2
                "
                            >
                                Logged in as
                            </div>

                            <div
                                className="
                  text-lg
                  font-semibold
                "
                            >
                                {account?.username ?? "Guest"}
                            </div>

                            <div
                                className="
                  text-cyan-400
                  text-sm
                  mt-2
                "
                            >
                                {account?.accountType ?? "PERSONAL"}
                            </div>

                            <div
                                className="
                  text-slate-300
                  text-sm
                "
                            >
                                {account?.subscription.plan ?? "STANDARD"}
                            </div>
                        </div>

                    </aside>
                </div>
            )}
        </>
    );
}
