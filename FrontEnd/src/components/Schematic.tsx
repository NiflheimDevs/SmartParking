import { useEffect, useState } from "react";
import { getParkingSpaces } from "../api";

interface Space {
  id: number;
  spot_number: string;
  is_occupied: boolean;
  last_updated: string;
}


const Schematic = () => {
  const [spaces, setSpaces] = useState<Space[]>([]);

  const loadSpaces = async () => {
    try {
      const data = await getParkingSpaces(); // ← direct array returned
      setSpaces(data);
    } catch (err) {
      console.error("Error loading parking spaces:", err);
    }
  };

  useEffect(() => {
    loadSpaces();                           // Initial load
    const interval = setInterval(loadSpaces, 5000);  // Auto-refresh
    return () => clearInterval(interval);
  }, []);

  return (
    <div className="bg-slate-800/60 rounded-xl p-6 border border-white/10 shadow">
      <h3 className="text-cyan-400 font-semibold mb-4">Parking Lot Schematic</h3>


      <div className="grid grid-cols-[repeat(auto-fill,minmax(90px,1fr))] gap-3">
        {spaces.map((s) => (
          <div
            key={s.id}
            className={`h-[90px] rounded-lg flex items-center justify-center font-bold text-sm 
              transition transform hover:scale-105 shadow
              ${
                s.is_occupied
                  ? "bg-red-400 text-white"
                  : "bg-green-400 text-slate-900"
              }`}
          >
            {s.spot_number}
          </div>
        ))}
      </div>
    </div>
  );
};

export default Schematic;