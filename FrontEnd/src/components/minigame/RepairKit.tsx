'use client';

import React, { useEffect, useRef, useState } from 'react';

interface RepairKitProps {
  onWin?: () => void;
  onLose?: () => void;
}

const RepairKit: React.FC<RepairKitProps> = ({ onWin, onLose }) => {
  const [position, setPosition] = useState(0);
  const [slotPosition] = useState(() => Math.floor(Math.random() * 76) + 15); // 15–90%
  const [isGameOver, setIsGameOver] = useState(false); // ← single source of truth

  const positionRef = useRef(0);
  const intervalRef = useRef<number | null>(null);

  // One single place that ends the game
  const endGame = (won: boolean) => {
    if (isGameOver) return; // ← prevent double call
    setIsGameOver(true);

    if (won) {
      onWin?.();
      window.parent.postMessage({ type: "WIN" }, "*");
    } else {
      onLose?.();
      window.parent.postMessage({ type: "LOSE" }, "*");
    }

    if (intervalRef.current !== null) {
      clearInterval(intervalRef.current);
      intervalRef.current = null;
    }
  };

  const tick = () => {
    const step = Math.floor(Math.random() * 3) + 3;
    positionRef.current += step;
    setPosition(positionRef.current);

    // Auto-lose
    if (positionRef.current >= slotPosition + 7) {
      endGame(false);
    }
  };

  const startGame = () => {
    positionRef.current = 0;
    setPosition(0);
    setIsGameOver(false);

    if (intervalRef.current !== null) clearInterval(intervalRef.current);
    const speedMs = 10 + Math.random() * 10;
    intervalRef.current = window.setInterval(tick, speedMs);
  };

  useEffect(() => {
    startGame();

    const handleKey = (e: KeyboardEvent) => {
      if ((e.key === 'e' || e.key === 'E') && !isGameOver) {
        e.preventDefault();
        const won = positionRef.current >= slotPosition - 2 && positionRef.current <= slotPosition + 2;
        endGame(won);
      }
    };

    window.addEventListener('keydown', handleKey);
    return () => {
      window.removeEventListener('keydown', handleKey);
      if (intervalRef.current !== null) clearInterval(intervalRef.current);
    };
  }, []);

  const buttonBg = isGameOver && positionRef.current <= slotPosition + 2 && positionRef.current >= slotPosition - 2
    ? 'from-emerald-500 to-emerald-700'
    : 'from-red-600 to-red-800';

  const squareBg = isGameOver
    ? (positionRef.current >= slotPosition - 2 && positionRef.current <= slotPosition + 2
        ? 'from-emerald-400 to-cyan-600 shadow-emerald-400/90'
        : 'from-red-500 to-rose-700 shadow-red-500/90')
    : 'from-cyan-400 to-teal-600 shadow-cyan-400/80';

  return (
    <div className="flex items-center gap-12 p-12 bg-gradient-to-r from-slate-950/90 to-cyan-950/40 rounded-3xl border-2 border-cyan-800/50 shadow-2xl select-none">
      {/* E Button */}
      <div className={`flex items-center justify-center w-28 h-28 rounded-2xl bg-gradient-to-br ${isGameOver ? buttonBg : 'from-emerald-500 to-emerald-700'} text-white text-6xl font-black shadow-2xl`}>
        E
      </div>

      {/* Timeline */}
      <div className="relative w-full max-w-3xl h-24 bg-black/50 rounded-xl border-4 border-gray-700/60 overflow-hidden">
        {/* Moving Square */}
        <div
          className={`absolute top-1/2 w-20 h-20 rounded-xl bg-gradient-to-br ${squareBg} shadow-2xl flex items-center justify-center transition-transform duration-200`}
          style={{
            left: `${Math.min(position, 100)}%`,
            marginLeft: position <= 100 ? '-40px' : '0px',
            transform: `translateY(-50%) scale(${isGameOver && positionRef.current >= slotPosition - 2 && positionRef.current <= slotPosition + 2 ? 1.5 : 1})`,
          }}
        >
          {isGameOver && (positionRef.current >= slotPosition - 2 && positionRef.current <= slotPosition + 2
            ? <i className="fas fa-check text-4xl"></i>
            : <i className="fas fa-xmark text-4xl"></i>
          )}
        </div>

        {/* Success Slot */}
        <div
          className="absolute top-0 bottom-0 w-20 pointer-events-none"
          style={{ left: `${slotPosition}%`, marginLeft: '-40px' }}
        >
          <div className="h-full border-4 border-dashed border-cyan-500/80 rounded-xl relative">
            <div className="absolute inset-x-0 top-0 h-1 bg-gradient-to-r from-transparent via-cyan-400 to-transparent"></div>
            <div className="absolute inset-x-0 bottom-0 h-1 bg-gradient-to-r from-transparent via-cyan-400 to-transparent"></div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default RepairKit;