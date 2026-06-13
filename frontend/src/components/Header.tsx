import { Link } from "react-router-dom";

export default function Header() {
  const scrollToSection = (id: string) => {
    document.getElementById(id)?.scrollIntoView({
      behavior: "smooth",
      block: "start",
    });
  };
  return (
    <header
      className="sticky
            top-0
            z-30
            backdrop-blur-md
            bg-[#020617]/80
            border-b
            border-slate-800"
    >
      <div
        className="max-w-[1600px]
              mx-auto
              px-6
              md:px-10
              lg:px-12
              h-24
              flex
              items-center
              justify-between"
      >
        {/* LEFT */}

        <div className="flex
              items-center
              gap-14">
          <Link
            to="/"
            onClick={() =>
              window.scrollTo({
                top: 0,
                behavior: "smooth",
              })
            }
            className="flex
                  items-center
                  gap-4
                  font-bold
                  group"
          >
            <span
              className="text-4xl
                    transition
                    group-hover:scale-110"
            >
              ⚡
            </span>
          </Link>

          <nav
            className="hidden
                  md:flex
                  items-center
                  gap-10
                  text-xl"
          >
            <button
              onClick={() => scrollToSection("features")}
              className="text-slate-300
                    hover:text-cyan-400
                    transition"
            >
              Features
            </button>

            <button
              onClick={() => scrollToSection("stats")}
              className="text-slate-300
                    hover:text-cyan-400
                    transition"
            >
              Statistics
            </button>

            <button
              onClick={() => scrollToSection("about")}
              className="text-slate-300
                    hover:text-cyan-400
                    transition"
            >
              About
            </button>
          </nav>
        </div>

        {/* RIGHT */}

        <div className="flex
              items-center
              gap-4">
          <Link
            to="/login"
            className="px-6
                  py-3
                  rounded-2xl
                  text-lg
                  text-slate-300
                  hover:text-cyan-400
                  hover:bg-cyan-500/10
                  transition"
          >
            Sign in
          </Link>

          <Link
            to="/register"
            className="px-8
                  py-3
                  rounded-2xl
                  text-lg
                  font-semibold
                  bg-cyan-500
                  text-slate-900
                  hover:bg-cyan-400
                  transition
                  shadow-lg
                  shadow-cyan-500/20"
          >
            Sign up
          </Link>
        </div>
      </div>
    </header>
  );
}
