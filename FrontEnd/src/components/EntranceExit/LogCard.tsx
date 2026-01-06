interface LogCardProps {
  log: any;
}

const LogCard = ({ log }: LogCardProps) => {
  const isExited = log.exit_time && !log.exit_time.startsWith("0001");

  return (
    <div className="border border-slate-700 bg-slate-900/70 rounded-xl p-4 shadow-md hover:shadow-lg transition">
      {/* HEADER */}
      <div className="flex justify-between items-center mb-3">
        <h2 className="text-lg font-semibold text-accent">Log #{log.id}</h2>
        <span
          className={`px-2 py-1 rounded text-xs ${
            isExited
              ? "bg-green-700/40 text-green-300"
              : "bg-yellow-700/40 text-yellow-300"
          }`}
        >
          {isExited ? "Exited" : "In Parking"}
        </span>
      </div>

      {/* VEHICLE INFO */}
      <div className="mb-3">
        <p className="text-sm text-slate-300"><span className="font-semibold">Plate:</span> {log.vehicle?.plate}</p>
        <p className="text-sm text-slate-300"><span className="font-semibold">RFID:</span> {log.vehicle?.rfid_id}</p>
        <p className="text-sm text-slate-300"><span className="font-semibold">Owner:</span> {log.vehicle?.owner_name}</p>
        <p className="text-sm text-slate-300"><span className="font-semibold">Contact:</span> {log.vehicle?.owner_contact}</p>
      </div>

      {/* TIMES */}
      <div className="border-t border-slate-700 pt-3 mt-3 text-sm">
       {!isExited && (
          <p className="text-slate-400">
            <span className="font-semibold text-slate-200">Space Number:</span>{" "}
            {log.parking_spot?.spot_number}
          </p>
        )}

        <p className="text-slate-400">
          <span className="font-semibold text-slate-200">Entrance:</span> {new Date(log.entrance_time).toLocaleString()}
        </p>

        <p className="text-slate-400">
          <span className="font-semibold text-slate-200">Exit:</span>{" "}
          {isExited ? new Date(log.exit_time).toLocaleString() : "—"}
        </p>

        <p className="text-slate-400 mt-2">
          <span className="font-semibold text-slate-200">Price:</span>{" "}
          {log.price ? log.price.toLocaleString() + " تومان" : "—"}
        </p>
      </div>
    </div>
  );
};

export default LogCard;