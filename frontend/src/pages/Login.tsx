import { Link, useNavigate } from "react-router-dom";
import { useState } from "react";
import { loginUser } from "../services/api";

export default function Login() {
  const navigate = useNavigate();

  const [email, setEmail] = useState("");

  const [password, setPassword] = useState("");

  const [error, setError] = useState("");

  const [isSubmitting, setIsSubmitting] = useState(false);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();

    setError("");

    if (!email.trim()) {
      setError("Email is required");
      return;
    }

    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

    if (!emailRegex.test(email)) {
      setError("Please enter a valid email address");
      return;
    }

    if (!password) {
      setError("Password is required");
      return;
    }

    if (password.length < 8) {
      setError("Password must contain at least 8 characters");
      return;
    }

    setIsSubmitting(true);

    try {
      await loginUser({
        email,
        password,
      });

      navigate("/account");
    } catch (err) {
      setError(err instanceof Error ? err.message : "Login failed");
    } finally {
      setIsSubmitting(false);
    }
  };

  return (
    <div
      className="min-h-[80vh]
            flex
            items-center
            justify-center
            px-6"
    >
      <div
        className="w-full
              max-w-md"
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
            Dorm Energy Cloud
          </h1>

          <p className="text-slate-300">Sign in to your account</p>
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
            <label
              className="block
                    mb-2
                    text-sm
                    text-slate-300"
            >
              Email
            </label>

            <input
              type="email"
              placeholder="name@example.com"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              className="w-full
                    px-4
                    py-3
                    rounded-xl
                    bg-[#020617]
                    border
                    border-slate-700
                    text-white
                    placeholder:text-slate-300
                    focus:border-cyan-500
                    outline-none"
            />
          </div>

          <div>
            <label
              className="block
                    mb-2
                    text-sm
                    text-slate-300"
            >
              Password
            </label>

            <input
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              className="w-full
                    px-4
                    py-3
                    rounded-xl
                    bg-[#020617]
                    border
                    border-slate-700
                    text-white
                    placeholder:text-slate-300
                    focus:border-cyan-500
                    outline-none"
            />

            <div
              className="mt-2
                    text-xs
                    text-slate-300"
            >
              Password must contain at least 8 characters
            </div>
          </div>

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
            {isSubmitting ? "Signing in..." : "Sign in"}
          </button>
        </form>

        <div
          className="text-center
                mt-6
                text-slate-300"
        >
          New here?{" "}
          <Link
            to="/register"
            className="text-cyan-400
                  hover:text-cyan-300"
          >
            Create account
          </Link>
        </div>
      </div>
    </div>
  );
}
