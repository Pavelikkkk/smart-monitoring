import { Link, useNavigate } from "react-router-dom";
import { useState } from "react";
import { registerUser } from "../services/api";

export default function Register() {
  const navigate = useNavigate();

  const [username, setUsername] = useState("");

  const [email, setEmail] = useState("");

  const [password, setPassword] = useState("");

  const [accountType, setAccountType] = useState<"personal" | "business">(
    "personal",
  );

  const [error, setError] = useState("");

  const [isSubmitting, setIsSubmitting] = useState(false);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();

    setError("");
    setIsSubmitting(true);

    try {
      await registerUser({
        username,
        email,
        password,
        accountType: accountType === "business" ? "BUSINESS" : "PERSONAL",
      });

      navigate("/login");
    } catch (err) {
      setError(err instanceof Error ? err.message : "Registration failed");
    } finally {
      setIsSubmitting(false);
    }
  };

  return (
    <div
      className="min-h-screen
            flex
            items-center
            justify-center
            px-6"
    >
      <div
        className="w-full
              max-w-lg"
      >
        <div className="text-center
              mb-8">
          <div className="text-6xl
                mb-4">⚡</div>

          <h1
            className="text-4xl
                  font-bold
                  mb-3"
          >
            Create Account
          </h1>

          <p className="text-slate-300">Join Dorm Energy Cloud</p>
        </div>

        <form
          onSubmit={handleSubmit}
          className="bg-[#111827]
                border
                border-cyan-700/40
                rounded-3xl
                p-5
                space-y-6"
        >
          <div>
            <label className="block
                  mb-2
                  text-sm
                  text-slate-300">
              Username
            </label>

            <input
              value={username}
              onChange={(e) => setUsername(e.target.value)}
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
          </div>

          <div>
            <label className="block
                  mb-2
                  text-sm
                  text-slate-300">Email</label>

            <input
              type="email"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
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
          </div>

          <div>
            <label className="block
                  mb-2
                  text-sm
                  text-slate-300">
              Password
            </label>

            <input
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
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
          </div>

          <div>
            <label
              className="block
                    mb-4
                    text-sm
                    text-slate-300"
            >
              Account Type
            </label>

            <div className="grid
                  grid-cols-2
                  gap-4">
              <button
                type="button"
                onClick={() => setAccountType("personal")}
                className={`
                  p-4
                  rounded-2xl
                  border
                  transition
                  ${
                    accountType === "personal"
                      ? "border-cyan-500 bg-cyan-500/10"
                      : "border-slate-700"
                  }
                `}
              >
                <div className="text-3xl
                      mb-2">👤</div>

                <div className="font-semibold">Personal</div>

                <div className="text-xs
                      text-slate-300
                      mt-1">
                  Rooms & Devices
                </div>
              </button>

              <button
                type="button"
                onClick={() => setAccountType("business")}
                className={`
                  p-4
                  rounded-2xl
                  border
                  transition
                  ${
                    accountType === "business"
                      ? "border-cyan-500 bg-cyan-500/10"
                      : "border-slate-700"
                  }
                `}
              >
                <div className="text-3xl
                      mb-2">🏢</div>

                <div className="font-semibold">Business</div>

                <div className="text-xs
                      text-slate-300
                      mt-1">
                  Buildings & Analytics
                </div>
              </button>
            </div>
          </div>

          <button
            type="submit"
            disabled={isSubmitting}
            className="w-full
                  py-3
                  rounded-xl
                  bg-cyan-500
                  text-slate-900
                  font-semibold
                  hover:bg-cyan-400
                  transition"
          >
            {isSubmitting ? "Creating..." : "Create Account"}
          </button>

          {error && (
            <div
              className="p-3
                    rounded-xl
                    bg-red-500/10
                    border
                    border-red-500/30
                    text-red-400"
            >
              {error}
            </div>
          )}
        </form>

        <div
          className="text-center
                mt-6
                text-slate-300"
        >
          Already have an account?{" "}
          <Link
            to="/login"
            className="text-cyan-400
                  hover:text-cyan-300"
          >
            Sign in
          </Link>
        </div>
      </div>
    </div>
  );
}
