import { useState } from "react";
import { motion } from "framer-motion";
import { login } from "../../api";

export default function LoginPage({ switchToSignup }: { switchToSignup: () => void }) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");

  const handleLogin = async () => {
    try {
      if (!username || !password) {
        alert("Please enter both username and password");
        return;
      }

      const data = await login({ username, password });

      if (data?.token) {
        window.location.reload(); // Refresh app to re-check auth
      } else {
        alert("Invalid username or password");
      }
    } catch (err: any) {
      console.error("Login error:", err);
      alert(err?.message || "Login failed");
    }
  };

  return (
    <div className="min-h-screen flex items-center justify-center p-4">
      <motion.div
        initial={{ opacity: 0, y: 20 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ duration: 0.5 }}
        className="w-full max-w-md"
      >
        <div className="bg-slate-900/70 backdrop-blur-xl border border-slate-800 shadow-2xl rounded-2xl p-8">
          <h2 className="text-2xl font-semibold text-accent mb-6 text-center">
            Welcome Back
          </h2>

          <div className="grid gap-4">
            <input
              type="text"
              placeholder="Username"
              value={username}
              onChange={(e) => setUsername(e.target.value)}
              className="w-full px-3 py-2 rounded-lg bg-slate-800/50 text-slate-200 border border-slate-700 focus:border-accent focus:outline-none"
            />

            <input
              type="password"
              placeholder="Password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              className="w-full px-3 py-2 rounded-lg bg-slate-800/50 text-slate-200 border border-slate-700 focus:border-accent focus:outline-none"
            />

            <button
              onClick={handleLogin}
              className="w-full px-4 py-2 bg-accent text-slate-900 font-semibold rounded-xl hover:opacity-90 transition"
            >
              Login
            </button>
          </div>

          <p className="text-slate-400 text-sm text-center mt-6">
            Don't have an account?{" "}
            <span
              onClick={switchToSignup}
              className="text-accent cursor-pointer hover:underline"
            >
              Sign up
            </span>
          </p>
        </div>
      </motion.div>
    </div>
  );
}