import { Link } from "react-router-dom";
import { useState } from "react";

export default function Sidebar() {
    const [open, setOpen] =
        useState(false);

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
            border-cyan-900/40
            hover:bg-slate-700
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
              p-6
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
                                to="/"
                                className="
                  flex
                  items-center
                  gap-3
                "
                                onClick={() =>
                                    setOpen(false)
                                }
                            >
                                <span className="text-4xl">
                                    ⚡
                                </span>

                                <span
                                    className="
                    text-xl
                    font-bold
                  "
                                >
                                    Monitoring System
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

                        <div className="space-y-4 text-lg">

                            <Link
                                to="/buildings"
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="block hover:text-cyan-400"
                            >
                                🏢 Buildings
                            </Link>

                            <Link
                                to="/devices"
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="block hover:text-cyan-400"
                            >
                                📡 Devices
                            </Link>

                            <Link
                                to="/anomalies"
                                onClick={() =>
                                    setOpen(false)
                                }
                                className="block hover:text-cyan-400"
                            >
                                ⚠️ Anomalies
                            </Link>

                        </div>
                    </aside>
                </div>
            )}
        </>
    );
}