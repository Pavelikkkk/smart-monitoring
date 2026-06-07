type Props = {
  title: string;
  value: string | number;
};

export default function StatCard({
  title,
  value,
}: Props) {
  return (
    <div className="bg-[#111827] border border-cyan-900/40 rounded-2xl p-6">
      <p className="text-slate-500 text-sm">
        {title}
      </p>

      <h2 className="text-4xl font-bold mt-2">
        {value}
      </h2>
    </div>
  );
}