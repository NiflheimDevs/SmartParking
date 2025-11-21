import { useEffect, useState } from "react";

interface Slot {
  id: number;
  occupied: boolean;
}

const Schematic = () => {
  const [slots, setSlots] = useState<Slot[]>([]);

  useEffect(() => {
    renderSlots();
    const interval = setInterval(renderSlots, 5000);
    return () => clearInterval(interval);
  }, []);

  const renderSlots = () => {
    const temp: Slot[] = [];
    for (let i = 1; i <= 7; i++) {
      temp.push({ id: i, occupied: Math.random() > 0.5 });
    }
    setSlots(temp);
  };

  return (
    <div className="bg-slate-800/60 rounded-xl p-6 border border-white/10 shadow">
      <h3 className="text-cyan-400 font-semibold mb-4">Parking Lot Schematic</h3>
      <div className="grid grid-cols-[repeat(auto-fill,minmax(90px,1fr))] gap-3">
        {slots.map((s) => (
          <div
            key={s.id}
            className={`h-[90px] rounded-lg flex items-center justify-center font-bold text-sm transition transform hover:scale-105 shadow ${
              s.occupied
                ? "bg-red-400 text-white"
                : "bg-green-400 text-slate-900"
            }`}
          >
            S{s.id}
          </div>
        ))}
      </div>
    </div>
  );
};

export default Schematic;
