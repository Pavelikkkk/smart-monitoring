import { Link } from "react-router-dom";

export default function Header() {
  return (
    <header
      className="
        sticky
        top-0
        z-30
        backdrop-blur-md
        bg-[#0B1220]/80
        border-b
        border-slate-800
      "
    >
      <div
        className="
          max-w-[1600px]
          mx-auto
          px-6
          md:px-10
          lg:px-12
          h-20
          flex
          items-center
          gap-10
        "
      >
        <Link
          to="/"
          className="
            flex
            items-center
            gap-3
            font-bold
            text-xl
          "
        >
          <span className="text-3xl">
            ⚡
          </span>

        </Link>

        <nav
          className="
            flex
            items-center
            gap-8
            text-lg
          "
        >
          <Link
            to="/buildings"
                      className="hover:text-cyan-400 transition"
          >
            Buildings
          </Link>

          <Link
            to="/devices"
                      className="hover:text-cyan-400 transition"
          >
            Devices
          </Link>

          <Link
            to="/anomalies"
                      className="hover:text-cyan-400 transition"
          >
            Anomalies
          </Link>
        </nav>
      </div>
    </header>
  );
}