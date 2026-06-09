import Header from "../components/Header";

type Props = {
  children: React.ReactNode;
};

export default function MainLayout({
  children,
}: Props) {
  return (
    <div className="min-h-screen bg-[#020617] text-white">

      <Header />

      <main
        className="
          max-w-[1600px]
          mx-auto
          px-6
          md:px-10
          lg:px-12
          py-8
        "
      >
        {children}
      </main>

    </div>
  );
}