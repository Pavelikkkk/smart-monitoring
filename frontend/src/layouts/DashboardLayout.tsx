
import Navbar from "../components/Navbar";

type Props = {
  children: React.ReactNode;
};

export default function DashboardLayout({
  children,
}: Props) {
  return (
    <div className="min-h-screen bg-[#020617] text-white">

      <Navbar />

      <main
        className="
          max-w-[1600px]
          mx-auto
          px-6
          md:px-10
          lg:px-12
          pb-10
        "
      >
        {children}
      </main>

    </div>
  );
}