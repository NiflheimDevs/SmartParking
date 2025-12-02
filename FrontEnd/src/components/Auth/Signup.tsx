import { useState, useEffect, useRef } from "react";
import { motion } from "framer-motion";
import { signup } from "../../api";

export default function SignupPage({ switchToLogin }: { switchToLogin: () => void }) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [showMiniGame, setShowMiniGame] = useState(false);

  const iframeRef = useRef<HTMLIFrameElement>(null);

  useEffect(() => {
    if (showMiniGame && iframeRef.current) {
      iframeRef.current.focus();
    }
  }, [showMiniGame]);

  const handleSignupClick = () => {
    if (!username || !password) {
      alert("Please enter both username and password");
      return;
    }
    setShowMiniGame(true);
  };

  useEffect(() => {
    const handleMessage = (event: MessageEvent) => {

      if (event.data?.type === "WIN") {
        setShowMiniGame(false);
        signup({ username, password })
          .then(() => switchToLogin())
          .catch((err: any) => alert(err?.message || "Signup failed"));
      }
      if (event.data?.type === "LOSE") {
        setShowMiniGame(false);
        alert("You Sucks Idiot!!!");
      }
    };

    window.addEventListener("message", handleMessage);
    return () => window.removeEventListener("message", handleMessage);
  }, [username, password, switchToLogin]);

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
              onClick={handleSignupClick}
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

      {/* Mini-game iframe overlay */}
      {showMiniGame && (
        <div className="absolute inset-0 bg-black/80 flex items-center justify-center z-50">
          <iframe
            ref={iframeRef}
            src="/minigame/RepairKit.html" // Path to your HTML mini-game
            className="w-[900px] h-[300px] rounded-xl border-0"
            tabIndex={0}
          />
        </div>
      )}
    </div>
  );
}