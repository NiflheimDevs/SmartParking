import { useState } from "react";
import {
  createVehicle,
  getVehicle,
  updateVehicle,
  deleteVehicle,
} from "../../api";

interface Props {
  action: "create" | "read" | "update" | "delete";
  onResult: (r: any) => void;
  onDebug: (r: string) => void;
}

// type Vehicle = {
//   plate: string;
//   rfid: string;
//   owner: string;
// };

const CrudForm = ({ action, onResult, onDebug }: Props) => {
  const [vehicleId, setVehicleId] = useState("");
  const [plate, setPlate] = useState("");
  const [rfid, setRfid] = useState("");
  const [owner, setOwner] = useState("");
  const [loading, setLoading] = useState(false);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoading(true);
    try {
      let res;
      if (action === "create") {
        res = await createVehicle({ plate, rfid, owner });
        // Reset form after successful create
        setPlate("");
        setRfid("");
        setOwner("");
      } else if (action === "read") {
        res = await getVehicle(vehicleId);
      } else if (action === "update") {
        res = await updateVehicle(vehicleId, { plate, rfid, owner });
      } else if (action === "delete") {
        if (!confirm("Delete this vehicle?")) return;
        res = await deleteVehicle(vehicleId);
      }
      onResult(res?.data);
      onDebug(JSON.stringify(res?.data, null, 2));
    } catch (err: any) {
      onDebug("Error: " + err.message);
    } finally {
      setLoading(false);
    }
  };

  return (
    <form onSubmit={handleSubmit} className="flex flex-col gap-2">
      <h3 className="text-cyan-400 font-semibold mb-2 capitalize">
        {action} Vehicle
      </h3>

      {action !== "create" && (
        <input
          type="text"
          placeholder="Vehicle ID"
          value={vehicleId}
          onChange={(e) => setVehicleId(e.target.value)}
          className="input"
          required
        />
      )}
      {(action === "create" || action === "update") && (
        <>
          <input
            type="text"
            placeholder="License Plate"
            value={plate}
            onChange={(e) => setPlate(e.target.value)}
            className="input"
            required
          />
          <input
            type="text"
            placeholder="RFID Tag"
            value={rfid}
            onChange={(e) => setRfid(e.target.value)}
            className="input"
            required
          />
          <input
            type="text"
            placeholder="Owner Name"
            value={owner}
            onChange={(e) => setOwner(e.target.value)}
            className="input"
            required
          />
        </>
      )}
      <button
        type="submit"
        disabled={loading}
        className="btn-accent mt-2 disabled:opacity-50"
      >
        {loading ? "Loading..." : "Submit"}
      </button>
    </form>
  );
};

export default CrudForm;
