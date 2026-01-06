import { useEffect, useState } from "react";
import { getParkingSpaces, controlgate } from "../api";

interface Vehicle {
  id: number;
  owner_name: string;
  owner_contact: string;
  plate: string;
  rfid_id: string;
  vehicle_type: string;
  created_at: string;
}

interface Space {
  number: string;
  is_occupied: boolean;
  vehicle: Vehicle | null;
  entrance_time: string | null;
}

const Schematic = () => {
  const [spaces, setSpaces] = useState<Space[]>([]);
  const [selectedVehicle, setSelectedVehicle] = useState<Vehicle | null>(null);
  const [selectedVehicleTime, setSelectedVehicleTime] = useState<string | null>(null);
  const [gateLoading, setGateLoading] = useState<string | null>(null);

  const loadSpaces = async () => {
    try {
      const data = await getParkingSpaces();
      setSpaces(data);
    } catch (err) {
      console.error("Error loading parking spaces:", err);
    }
  };

  useEffect(() => {
    loadSpaces();
    const interval = setInterval(loadSpaces, 20000);
    return () => clearInterval(interval);
  }, []);

  const handleSpotClick = (space: Space) => {
    if (space.vehicle) {
      setSelectedVehicle(space.vehicle);
      setSelectedVehicleTime(space.entrance_time);
    }
  };

  const triggerGate = async (label: string, gate: "entrance" | "exit", state: boolean) => {
    try {
      setGateLoading(label);
      await controlgate(gate, state);
      await new Promise((resolve) => setTimeout(resolve, 5000));
    } catch (err) {
      console.error(err);
    } finally {
      setGateLoading(null);
    }
  };

  const formatTime = (isoString: string | null) => {
  if (!isoString) return "-";
  const date = new Date(isoString);
  return date.toLocaleString("en-US", {
    year: "numeric",
    month: "short",
    day: "numeric",
    hour: "2-digit",
    minute: "2-digit",
  });
};

  return (
    <>
      {/* -------------------- Schematic -------------------- */}
      <div className="bg-slate-800/60 rounded-xl p-6 border border-white/10 shadow">
        <h3 className="text-cyan-400 font-semibold mb-4">Parking Lot Schematic</h3>

        <div className="grid grid-cols-[repeat(auto-fill,minmax(90px,1fr))] gap-3">
          {spaces.map((s) => (
            <div
              key={s.number}
              onClick={() => handleSpotClick(s)}
              className={`h-[90px] rounded-lg flex items-center justify-center font-bold text-sm cursor-pointer 
                transition transform hover:scale-105 shadow
                ${
                  s.is_occupied
                    ? "bg-red-400 text-white"
                    : "bg-green-400 text-slate-900"
                }`}
            >
              {s.number}
            </div>
          ))}
        </div>
      </div>

      {/* -------------------- Gate Control Panel -------------------- */}
      <div className="bg-slate-800/60 rounded-xl p-6 border border-white/10 shadow mt-6">
        <h3 className="text-cyan-400 font-semibold mb-4">Gate Controls</h3>

        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">

          {/* Entrance Gate */}
          <div className="bg-slate-900/70 p-5 rounded-xl border border-white/10 shadow-lg">
            <h4 className="text-slate-200 font-semibold mb-4">Entrance Gate</h4>

            <div className="space-y-2">
              <button
                className="w-full bg-green-400 hover:bg-green-500 text-slate-900 font-semibold py-2 rounded-lg transition shadow disabled:bg-green-700/60"
                disabled={gateLoading !== null}
                onClick={() => triggerGate("Opening Entrance", "entrance", true)}
              >
                {gateLoading === "Opening Entrance" ? "Opening..." : "Open Gate"}
              </button>

              <button
                className="w-full bg-red-400 hover:bg-red-500 text-white font-semibold py-2 rounded-lg transition shadow disabled:bg-red-700/60"
                disabled={gateLoading !== null}
                onClick={() => triggerGate("Closing Entrance", "entrance", false)}
              >
                {gateLoading === "Closing Entrance" ? "Closing..." : "Close Gate"}
              </button>
            </div>
          </div>

          {/* Exit Gate */}
          <div className="bg-slate-900/70 p-5 rounded-xl border border-white/10 shadow-lg">
            <h4 className="text-slate-200 font-semibold mb-4">Exit Gate</h4>

            <div className="space-y-2">
              <button
                className="w-full bg-green-400 hover:bg-green-500 text-slate-900 font-semibold py-2 rounded-lg transition shadow disabled:bg-green-700/60"
                disabled={gateLoading !== null}
                onClick={() => triggerGate("Opening Exit", "exit", true)}
              >
                {gateLoading === "Opening Exit" ? "Opening..." : "Open Gate"}
              </button>

              <button
                className="w-full bg-red-400 hover:bg-red-500 text-white font-semibold py-2 rounded-lg transition shadow disabled:bg-red-700/60"
                disabled={gateLoading !== null}
                onClick={() => triggerGate("Closing Exit", "exit", false)}
              >
                {gateLoading === "Closing Exit" ? "Closing..." : "Close Gate"}
              </button>
            </div>
          </div>

        </div>
      </div>

      {/* -------------------- Vehicle Modal -------------------- */}
      {selectedVehicle && (
        <div
          className="fixed inset-0 bg-black/50 backdrop-blur-sm flex items-center justify-center p-4 z-50"
          onClick={() => setSelectedVehicle(null)}
        >
          <div
            className="bg-slate-900 text-slate-100 rounded-2xl p-6 w-full max-w-sm shadow-2xl border border-white/10"
            onClick={(e) => e.stopPropagation()}
          >
            <h2 className="text-xl font-semibold mb-4 text-cyan-300">
              Vehicle Details
            </h2>

            <div className="space-y-2 text-sm">
              <div className="flex justify-between">
                <span className="text-slate-400">Owner:</span>
                <span className="font-medium">{selectedVehicle.owner_name}</span>
              </div>

              <div className="flex justify-between">
                <span className="text-slate-400">Contact:</span>
                <span className="font-medium">{selectedVehicle.owner_contact}</span>
              </div>

              <div className="flex justify-between">
                <span className="text-slate-400">Plate:</span>
                <span className="font-medium">{selectedVehicle.plate}</span>
              </div>

              <div className="flex justify-between">
                <span className="text-slate-400">RFID:</span>
                <span className="font-medium">{selectedVehicle.rfid_id}</span>
              </div>

              <div className="flex justify-between">
                <span className="text-slate-400">Type:</span>
                <span className="font-medium capitalize">
                  {selectedVehicle.vehicle_type}
                </span>
              </div>

              <div className="flex justify-between">
                <span className="text-slate-400">Entered at:</span>
                <span className="font-medium">{formatTime(selectedVehicleTime)}</span>
              </div>

            </div>

            <button
              onClick={() => setSelectedVehicle(null)}
              className="mt-6 w-full bg-cyan-500 hover:bg-cyan-600 text-slate-900 font-semibold py-2 rounded-xl transition"
            >
              Close
            </button>
          </div>
        </div>
      )}
    </>
  );
};

export default Schematic;