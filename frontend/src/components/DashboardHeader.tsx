import Sidebar from "./Sidebar";

export default function DashboardHeader() {
  return (
    <header
      className="sticky
            top-0
            z-40
            backdrop-blur-md
            bg-[#020617]/80
            border-b
            border-slate-800
            mb-8"
    >
      <div
        className="max-w-[1600px]
              mx-auto
              px-6
              md:px-10
              lg:px-12
              h-20
              flex
              items-center
              gap-4"
      >
        <Sidebar />

        <div
          className="flex
                items-center
                gap-3
                font-bold
                text-xl"
        >
          <span className="text-3xl">⚡</span>

          <span>Dorm Energy</span>
        </div>
      </div>
    </header>
  );
}
