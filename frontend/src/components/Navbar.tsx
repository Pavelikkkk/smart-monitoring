import { NavLink } from "react-router-dom";

export default function Navbar() {
  const linkClass = ({ isActive }: { isActive: boolean }) =>
    isActive
    ? "text-orange-500 font-semibold"
    : "text-slate-600 hover:text-orange-500 transition";

  return (
    <nav
      className="
        flex
        flex-col
        md:flex-row
        md:items-center
        gap-4
        md:gap-8
        mb-8
        border-b
        border-cyan-900/40
        pb-4
      "
    >
      <div
        className="
          text-xl
          font-bold
          text-cyan-400
        "
      >
        ⚡ Dorm Energy Monitoring
      </div>

      <div
        className="
          flex
          flex-wrap
          gap-4
          md:gap-8
        "
      >
        <NavLink to="/" className={linkClass}>
          Home
        </NavLink>

        <NavLink to="/buildings" className={linkClass}>
          Buildings
        </NavLink>

        <NavLink to="/analytics" className={linkClass}>
          Analytics
        </NavLink>

        <NavLink to="/anomalies" className={linkClass}>
          Anomalies
        </NavLink>

        <NavLink to="/devices" className={linkClass}>
          Devices
        </NavLink>
      </div>
    </nav>
  );
}