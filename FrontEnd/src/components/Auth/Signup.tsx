import { useState } from "react";
import { motion } from "framer-motion";
import { signup } from "../../api";

export default function SignupPage({ switchToLogin }: { switchToLogin: () => void }) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");

  const handleSignup = async () => {
    try {
      if (!username || !password) {
        alert("Please enter both username and password");
        return;
      }

      await signup({ username, password });
      switchToLogin();
    } catch (err: any) {
      console.error("Signup error:", err);
      alert(err?.message || "Signup failed");
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
            Create Your Account
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
              onClick={handleSignup}
              className="w-full px-4 py-2 bg-accent text-slate-900 font-semibold rounded-xl hover:opacity-90 transition"
            >
              Sign Up
            </button>
          </div>

          <p className="text-slate-400 text-sm text-center mt-6">
            Already have an account?{" "}
            <span
              onClick={switchToLogin}
              className="text-accent cursor-pointer hover:underline"
            >
              Log in
            </span>
          </p>
        </div>
      </motion.div>
    </div>
  );
}