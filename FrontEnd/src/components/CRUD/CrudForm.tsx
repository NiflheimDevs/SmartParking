import { useEffect, useState } from "react";
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
  openDeleteModal: (id: string | number) => void;
}

const CrudForm = ({ action, onResult, onDebug, openDeleteModal }: Props) => {
  const [vehicleId, setVehicleId] = useState("");

  const [ownerName, setOwnerName] = useState("");
  const [ownerContact, setOwnerContact] = useState("");
  const [plate, setPlate] = useState("");
  const [rfid, setRfid] = useState("");
  const [vehicleType, setVehicleType] = useState("");

  const [loading, setLoading] = useState(false);

  const resetFields = () => {
    setOwnerName("");
    setOwnerContact("");
    setPlate("");
    setRfid("");
    setVehicleType("");
  };

  // LISTEN FOR GLOBAL DELETE CONFIRM
  useEffect(() => {
    const handler = async (e: any) => {
      const id = e.detail.id;
      setLoading(true);

      try {
        const res = await deleteVehicle(id);
        onResult(res);
        onDebug(JSON.stringify(res, null, 2));
      } catch (err: any) {
        const msg = err.response?.data || err.message;
        onDebug("Error: " + JSON.stringify(msg, null, 2));
      }

      setLoading(false);
    };

    document.addEventListener("confirmDeleteVehicle", handler);
    return () => document.removeEventListener("confirmDeleteVehicle", handler);
  }, []);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoading(true);

    try {
      let res;

      if (action === "create") {
        res = await createVehicle({
          owner_name: ownerName,
          owner_contact: ownerContact,
          plate,
          rfid_id: rfid,
          vehicle_type: vehicleType,
        });

        resetFields();
      }

      else if (action === "read") {
        res = await getVehicle(vehicleId);
      }

      else if (action === "update") {
        res = await updateVehicle(vehicleId, {
          owner_name: ownerName,
          owner_contact: ownerContact,
          plate,
          rfid_id: rfid,
          vehicle_type: vehicleType,
        });
      }

      else if (action === "delete") {
        openDeleteModal(vehicleId);
        setLoading(false);
        return;
      }

      onResult(res);
      onDebug(JSON.stringify(res, null, 2));
    } catch (err: any) {
      const msg = err.response?.data || err.message;
      onDebug("Error: " + JSON.stringify(msg, null, 2));
    }

    setLoading(false);
  };

  return (
    <form onSubmit={handleSubmit} className="flex flex-col gap-3">
      
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
            placeholder="Owner Name"
            value={ownerName}
            onChange={(e) => setOwnerName(e.target.value)}
            className="input"
            required
          />

          <input
            type="text"
            placeholder="Owner Contact"
            value={ownerContact}
            onChange={(e) => setOwnerContact(e.target.value)}
            className="input"
            required
          />

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
            placeholder="Vehicle Type"
            value={vehicleType}
            onChange={(e) => setVehicleType(e.target.value)}
            className="input"
            required
          />
        </>
      )}

      <button
        type="submit"
        disabled={loading}
        className="btn-accent mt-3 disabled:opacity-50"
      >
        {loading ? "..." : "Submit"}
      </button>
    </form>
  );
};

export default CrudForm;
