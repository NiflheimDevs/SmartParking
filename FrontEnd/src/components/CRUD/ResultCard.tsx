interface ResultCardProps {
  data: any;
}

const ResultCard = ({ data }: ResultCardProps) => {
  if (!data) return null;

  return (
    <div className="border border-slate-700 bg-slate-900/70 rounded-xl p-4 shadow-md hover:shadow-lg transition">
        <>
          {/* Vehicle Card */}
          <div className="flex justify-between items-center mb-3">
            <h2 className="text-lg font-semibold text-accent">Vehicle</h2>
            <span className="px-2 py-1 rounded text-xs bg-blue-700/40 text-blue-300">
              {data.vehicle_type?.toUpperCase()}
            </span>
          </div>

          <div className="mb-3 space-y-1">
            <p className="text-sm text-slate-300">
              <span className="font-semibold">Plate:</span> {data.plate}
            </p>
            <p className="text-sm text-slate-300">
              <span className="font-semibold">RFID:</span> {data.rfid_id}
            </p>
            <p className="text-sm text-slate-300">
              <span className="font-semibold">Owner:</span> {data.owner_name}
            </p>
            <p className="text-sm text-slate-300">
              <span className="font-semibold">Contact:</span> {data.owner_contact}
            </p>
          </div>

          <div className="border-t border-slate-700 pt-3 mt-3 text-sm">
            <p className="text-slate-400">
              <span className="font-semibold text-slate-200">Created At:</span>{" "}
              {new Date(data.created_at).toLocaleString()}
            </p>
          </div>
        </>
    </div>
  );
};

export default ResultCard;
