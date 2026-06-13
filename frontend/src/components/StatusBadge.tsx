type Props = {
  online: boolean;
};

export default function StatusBadge({ online }: Props) {
  return (
    <div className="flex
          items-center
          gap-2">
      <div
        className={`
          w-3
          h-3
          rounded-full
          ${online ? "bg-green-400" : "bg-red-400"}
        `}
      />

      <span>{online ? "Online" : "Offline"}</span>
    </div>
  );
}
