'use client';

import React, { useState, useEffect, useRef, useCallback } from 'react';

const LETTER_VALUES = ['q', 'w', 'e', 'r', 'a', 's', 'd'] as const;
type LetterValue = typeof LETTER_VALUES[number];

interface Letter {
  value: LetterValue;
  state: '' | 'done' | 'fail';
}

// Static configuration - no settings needed
const TOTAL_SECONDS = 7;
const NUM_LETTERS = 20;
const GRID_COLS = 6;
const GRID_ROWS = 4;

interface ChoppingGameProps {
  onGameEnd: (result: 'win' | 'lose') => void;
}

const ChoppingGame: React.FC<ChoppingGameProps> = ({ onGameEnd }) => {
  const [letters, setLetters] = useState<Letter[]>([]);
  const [currentIndex, setCurrentIndex] = useState(0); // Track which letter is active
  const [isLocked, setIsLocked] = useState(false);
  const [outcome, setOutcome] = useState<'' | 'win' | 'lose'>('');
  const [timeLeftPercent, setTimeLeftPercent] = useState(100);

  const timerRef = useRef<number | null>(null);
  const progressRef = useRef<number | null>(null);

  const generateLetters = useCallback(() => {
    const newLetters: Letter[] = [];
    for (let i = 0; i < NUM_LETTERS; i++) {
      const randomValue = LETTER_VALUES[Math.floor(Math.random() * LETTER_VALUES.length)];
      newLetters.push({ value: randomValue, state: '' });
    }
    setLetters(newLetters);
    setCurrentIndex(0); // Reset index
  }, []);

  const resetGame = useCallback(() => {
    setIsLocked(false);
    setOutcome('');
    setTimeLeftPercent(100);
    generateLetters();
    runTimer();
  }, [generateLetters]);

  const runTimer = useCallback(() => {
    if (timerRef.current !== null) clearTimeout(timerRef.current);
    if (progressRef.current !== null) clearTimeout(progressRef.current);

    setTimeLeftPercent(100);

    // Start progress bar animation
    progressRef.current = setTimeout(() => {
      setTimeLeftPercent(0);
    }, 50);

    // Game over by time
    timerRef.current = setTimeout(() => {
      endGame('lose');
    }, TOTAL_SECONDS * 1000);
  }, []);

  const endGame = (result: 'win' | 'lose') => {
    if (isLocked) return;
    setIsLocked(true);
    setOutcome(result);

    if (timerRef.current !== null) clearTimeout(timerRef.current);
    if (progressRef.current !== null) clearTimeout(progressRef.current);

    onGameEnd(result);
  };

  const handleKeyDown = useCallback((e: KeyboardEvent) => {
    if (isLocked || currentIndex >= NUM_LETTERS) return;

    const key = e.key.toLowerCase();
    if (!LETTER_VALUES.includes(key as LetterValue)) return;

    const targetLetter = letters[currentIndex];

    if (targetLetter.value === key) {
      // Correct key!
      setLetters(prev => {
        const updated = [...prev];
        updated[currentIndex] = { ...updated[currentIndex], state: 'done' };
        return updated;
      });

      const nextIndex = currentIndex + 1;
      setCurrentIndex(nextIndex);

      if (nextIndex >= NUM_LETTERS) {
        endGame('win');
      }
    } else {
      // Wrong key
      setLetters(prev => {
        const updated = [...prev];
        updated[currentIndex] = { ...updated[currentIndex], state: 'fail' };
        return updated;
      });
      endGame('lose');
    }
  }, [letters, currentIndex, isLocked, onGameEnd]);

  useEffect(() => {
    resetGame();
  }, [resetGame]);

  useEffect(() => {
    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [handleKeyDown]);

  useEffect(() => {
    return () => {
      if (timerRef.current !== null) clearTimeout(timerRef.current);
      if (progressRef.current !== null) clearTimeout(progressRef.current);
    };
  }, []);

  useEffect(() => {
    resetGame();
  }, [resetGame]);

  useEffect(() => {
    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [handleKeyDown]);

  const rows: Letter[][] = [];
  for (let i = 0; i < GRID_ROWS; i++) {
    rows.push(letters.slice(i * GRID_COLS, (i + 1) * GRID_COLS));
  }

  return (
    <div className="fixed inset-0 bg-gradient-to-b from-[#151b30]/95 to-[#151b30]/95 bg-cover flex items-center justify-center">
      <div className="relative flex flex-col items-center bg-[#071320] rounded-lg outline outline-1 outline-gray-600 max-w-4xl w-full mx-4 max-h-screen overflow-hidden shadow-2xl">
        <div className="w-full flex items-center gap-4 px-6 py-5">
          <div>
            <h2 className="text-2xl font-bold text-[#54ffa4] [text-shadow:0_0_3px_#7fffbf]">Alphabet</h2>
            <p className="text-gray-400">Tap the letters in order</p>
          </div>
        </div>

        <div className="absolute top-4 right-6 pointer-events-none z-10">
          {outcome === 'win' && (
            <div className="flex items-center gap-3 bg-[#1e7d73] text-white px-5 py-3 rounded-lg animate-pulse">
              <i className="fas fa-circle-check text-3xl" />
              <p className="text-lg font-medium">Success!</p>
            </div>
          )}
          {outcome === 'lose' && (
            <div className="flex items-center gap-3 bg-[#5b1425] text-white px-5 py-3 rounded-lg animate-pulse">
              <i className="fas fa-circle-xmark text-3xl" />
              <p className="text-lg font-medium">Failed!</p>
            </div>
          )}
        </div>

        <div className="flex flex-col gap-3 px-6 pb-6 pt-2">
          {rows.map((row, rowIndex) => (
            <div key={rowIndex} className="flex justify-center gap-3">
              {row.map((letter, idx) => (
                <div
                  key={`${rowIndex}-${idx}`}
                  className={`
                    flex items-center justify-center text-6xl font-medium rounded-lg
                    w-24 h-24 border-2 border-[rgba(112,193,255,0.05)] bg-[rgba(22,40,52,0.15)]
                    text-[#70c1ff] backdrop-blur-sm transition-all duration-150
                    ${letter.state === 'done'
                      ? 'text-[#30ddbd] border-[rgba(48,221,189,0.3)] bg-[rgba(48,221,189,0.12)] backdrop-blur-md shadow-[0_0_12px_rgba(48,221,189,0.25)] [text-shadow:0_0_6px_rgba(48,221,189,0.4)]'
                      : ''
                    }
                    ${letter.state === 'fail'
                      ? 'text-[#ff5454] border-[rgba(255,84,84,0.3)] bg-[rgba(255,84,84,0.12)] backdrop-blur-md shadow-[0_0_12px_rgba(255,84,84,0.25)] [text-shadow:0_0_6px_rgba(255,84,84,0.4)]'
                      : ''
                    }
                  `}
                >
                  {letter.value.toUpperCase()}
                </div>
              ))}
            </div>
          ))}
        </div>

        <div className="w-full h-3 bg-[#242f3b]">
          <div
            className="h-full bg-orange-600 transition-all"
            style={{
              width: `${timeLeftPercent}%`,
              transition: `width ${TOTAL_SECONDS}s cubic-bezier(0.4, 1, 0.7, 0.93)`,
            }}
          />
        </div>
      </div>
    </div>
  );
};

export default ChoppingGame;