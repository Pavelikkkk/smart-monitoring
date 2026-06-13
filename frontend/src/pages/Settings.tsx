import { useEffect, useState } from "react";

import { getAccount, updateTelegramChatId } from "../services/api";

export default function Settings() {
  const [telegramChatId, setTelegramChatId] = useState("");

  const [status, setStatus] = useState("");

  useEffect(() => {
    getAccount()
      .then((account) => setTelegramChatId(account.telegramChatId ?? ""))
      .catch(() => setStatus("Failed to load settings"));
  }, []);

  const handleSave = async (e: React.FormEvent) => {
    e.preventDefault();
    setStatus("");

    try {
      await updateTelegramChatId(telegramChatId);
      setStatus("Settings saved");
    } catch (err) {
      setStatus(err instanceof Error ? err.message : "Failed to save settings");
    }
  };

  return (
    <div className="space-y-8">
      <h1 className="text-5xl
            font-bold">Settings</h1>

      <form
        onSubmit={handleSave}
        className="bg-[#111827]
              border
              border-cyan-700/40
              rounded-3xl
              p-5
              space-y-5"
      >
        <h2 className="text-3xl
              font-bold">Notifications</h2>

        <label className="block">
          <span className="block
                mb-2
                text-sm
                text-slate-300">
            Telegram Chat ID
          </span>

          <input
            value={telegramChatId}
            onChange={(e) => setTelegramChatId(e.target.value)}
            className="w-full
                  px-4
                  py-3
                  text-white
                  rounded-xl
                  bg-[#020617]
                  border
                  border-slate-700
                  focus:border-cyan-500
                  outline-none"
          />
        </label>

        <button
          type="submit"
          className="px-8
                py-3
                rounded-2xl
                bg-cyan-500
                text-slate-900
                font-semibold
                hover:bg-cyan-400
                transition"
        >
          Save
        </button>

        {status && <div className="text-slate-300">{status}</div>}
      </form>
    </div>
  );
}
