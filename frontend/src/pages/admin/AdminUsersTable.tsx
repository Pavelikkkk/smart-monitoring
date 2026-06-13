import type { AdminOverview } from "../../services/api";

export function AdminUsersTable({ overview }: { overview: AdminOverview }) {
  return (
    <div
      className="bg-[#111827]
      border
      border-cyan-700/40
      rounded-3xl
      p-5
      overflow-x-auto"
    >
      <h2
        className="text-3xl
        font-bold
        mb-5"
      >
        Users & Workspaces
      </h2>

      <table
        className="w-full
        text-left"
      >
        <thead className="text-slate-300">
          <tr>
            <th className="py-3">User</th>
            <th>Email</th>
            <th>Role</th>
            <th>Plan</th>
            <th>Workspace</th>
            <th>Data</th>
          </tr>
        </thead>

        <tbody>
          {overview.users.map((user) => (
            <tr
              key={user.id}
              className="border-t
              border-slate-800"
            >
              <td className="py-3">{user.username}</td>
              <td>{user.email}</td>
              <td>{user.role}</td>
              <td>{user.plan}</td>
              <td>{user.organizationName}</td>
              <td>
                {user.buildingsCount} buildings, {user.roomsCount} rooms,{" "}
                {user.devicesCount} devices
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}
