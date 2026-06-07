type Room = {
  roomId: string;
  power: number;
};

type Props = {
  rooms: Room[];
};

export default function TopRooms({
  rooms,
}: Props) {
  return (
    <div className="bg-slate-800 border border-slate-700 rounded-xl p-6">

      <h2 className="text-2xl font-bold mb-4">
        Top Active Rooms
      </h2>

      <div className="space-y-4">

        {rooms.map((room) => (
          <div
            key={room.roomId}
            className="flex justify-between"
          >
            <span>
              {room.roomId}
            </span>

            <span className="text-orange-200 font-bold">
              {room.power} W
            </span>
          </div>
        ))}

      </div>

    </div>
  );
}