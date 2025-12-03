import { useState } from "react";
import { motion } from "framer-motion";
import { signup } from "../../api";
import RepairKit from "../minigame/RepairKit";
import Footer from "../Footer";

export default function SignupPage({ switchToLogin }: { switchToLogin: () => void }) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [showMiniGame, setShowMiniGame] = useState(false);
  const [isSubmitting, setIsSubmitting] = useState(false);

  const handleSignupClick = () => {
    if (!username.trim() || !password.trim()) {
      alert("Please enter both username and password");
      return;
    }
    setShowMiniGame(true);
  };

  const handleWin = async () => {
    setIsSubmitting(true);
    setShowMiniGame(false);
    try {
      await signup({ username, password });
      switchToLogin();
    } catch (err: any) {
      alert(err?.message || "Signup failed");
    } finally {
      setIsSubmitting(false);
    }
  };

  const handleLose = () => {
    setShowMiniGame(false);
    alert("You Suck Idiot");
  };

  const closeMinigame = () => {
    if (!isSubmitting) {
      setShowMiniGame(false);
    }
  };

  return (
    <div className="min-h-screen flex flex-col items-center justify-center p-4 relative overflow-hidden">
      <motion.div
        initial={{ opacity: 0, y: 20 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ duration: 0.5 }}
        className="w-full max-w-md"
      >
        <div className="bg-slate-900/70 backdrop-blur-xl border border-slate-800 shadow-2xl rounded-2xl p-8">
          <img
            src="/logo-transparent.png"
            alt="Logo"
            className="mx-auto mb-6 w-850 h-auto object-contain"
          />
          
          <h2 className="text-2xl font-semibold text-accent mb-6 text-center">
            Create Your Account
          </h2>

          <div className="grid gap-4">
            <input
              type="text"
              placeholder="Username"
              value={username}
              onChange={(e) => setUsername(e.target.value)}
              disabled={showMiniGame}
              className="w-full px-3 py-2 rounded-lg bg-slate-800/50 text-slate-200 border border-slate-700 focus:border-accent focus:outline-none"
            />

            <input
              type="password"
              placeholder="Password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              disabled={showMiniGame}
              className="w-full px-3 py-2 rounded-lg bg-slate-800/50 text-slate-200 border border-slate-700 focus:border-accent focus:outline-none"
            />

            <button
              onClick={handleSignupClick}
              disabled={showMiniGame || isSubmitting}
              className="w-full px-4 py-2 bg-accent text-slate-900 font-semibold rounded-xl hover:opacity-90 transition"
            >
              {isSubmitting ? "Creating Account..." : "Sign Up"}
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

      <Footer/>

      {showMiniGame && (
        <div className="fixed inset-0 bg-black/90 flex items-center justify-center z-50 p-8">
          <div className="relative max-w-4xl w-full">
            <button
              onClick={closeMinigame}
              disabled={isSubmitting}
              className="absolute -top-16 right-0 text-slate-400 hover:text-white text-4xl transition z-10"
            >
              ×
            </button>

            <h3 className="text-center text-2xl font-bold text-cyan-400 mb-6 tracking-wider">
              RECAPTHA
            </h3>

            <div className="bg-gradient-to-r from-slate-950 via-blue-950 to-slate-950 p-8 rounded-2xl border-2 border-cyan-800/50 shadow-2xl">
              <RepairKit
                onWin={handleWin}
                onLose={handleLose}
              />
            </div>

            <p className="text-center text-slate-400 mt-6 text-sm">
              Press the key at the right moment
            </p>
          </div>
        </div>
      )}
    </div>
  );
}