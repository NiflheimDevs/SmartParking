'use client';

import { useState } from 'react';
import { toast } from 'react-toastify';
import ChoppingGame from './minigame/chopping';

export default function AccessDenied() {
  const [showGame, setShowGame] = useState(false);

  const handleAppeal = () => {
    setShowGame(true);
  };

  // This will be called when game ends (win or lose)
  const handleGameEnd = (result: 'win' | 'lose') => {
    if (result === 'win') {
      toast.success("Unban successful! Welcome back... for now.", {
        position: "top-right",
        autoClose: 5000,
        theme: "dark",
      });
      // Optionally redirect or do something else
      // window.location.href = '/';
    } else {
      toast.error("Appeal denied. Try harder next time!", {
        position: "top-right",
        autoClose: 5000,
        theme: "dark",
      });
    }

    setTimeout(() => {
      setShowGame(false);
    }, 500);
  };

  const handleCloseGame = () => {
    setShowGame(false);
  };

  if (showGame) {
    return (
      <>
        <ChoppingGame onGameEnd={handleGameEnd} />
        <div className="fixed bottom-8 left-1/2 transform -translate-x-1/2 bg-black/70 backdrop-blur-md text-white px-6 py-3 rounded-full text-sm font-medium cursor-pointer hover:bg-black/90 transition-all z-[9999]" onClick={handleCloseGame}>
          Press ESC to go back
        </div>
      </>
    );
  }

  return (
    <div className="min-h-screen flex items-center justify-center bg-gradient-to-br from-[#0f172a] to-[#1e293b] p-4" tabIndex={-1}>
      <div className="w-full max-w-2xl bg-slate-900/70 backdrop-blur-xl border border-slate-800 rounded-2xl shadow-2xl p-8 text-center">
        <h1 className="text-6xl font-extrabold text-red-500 mb-6 animate-pulse">403</h1>
        <h2 className="text-3xl font-bold text-yellow-400 mb-4">Banned Forever</h2>
        <p className="text-slate-400 mb-6 text-lg">
          Congratulations! You've officially achieved "Most Unwanted User" status.
          No cookies, no access, and definitely no sympathy.
        </p>
        <p className="text-slate-500 mb-6 text-sm">
          Maybe try being nice next time… or don’t. Your choice.
        </p>

        <button
          onClick={handleAppeal}
          className="px-6 py-3 bg-red-600 hover:bg-red-700 text-white font-bold rounded-xl shadow-lg transition"
        >
          Appeal for Unban
        </button>
      </div>
    </div>
  );
}
