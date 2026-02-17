interface InvoiceModalProps {
  log: any;
  onClose: () => void;
}

const InvoiceModal = ({ log, onClose }: InvoiceModalProps) => {
  const entrance = new Date(log.entrance_time);
  const exit = new Date(log.exit_time);
  const durationMs = exit.getTime() - entrance.getTime();
  const hours = Math.floor(durationMs / (1000 * 60 * 60));
  const minutes = Math.floor((durationMs / (1000 * 60)) % 60);

  return (
    <div className="fixed inset-0 z-50 bg-black/60 flex items-center justify-center">
      <div className="bg-white text-slate-900 w-full max-w-xl rounded-xl shadow-xl p-6 relative">

        {/* HEADER */}
        <div className="flex items-center justify-between border-b pb-4 mb-6">
          <div className="flex items-center gap-3">
            {/* LOGO */}
            <img
              src="/logo.png"
              alt="Slotify Logo"
              className="h-12 w-12 object-contain"
              onError={(e) => {
                (e.target as HTMLImageElement).style.display = "none";
              }}
            />

            {/* BUSINESS NAME */}
            <div>
              <h2 className="text-xl font-bold">Slotify</h2>
              <p className="text-xs text-slate-500">
                Smart Parking Management
              </p>
            </div>
          </div>

          <div className="text-right">
            <p className="text-sm font-semibold">Invoice</p>
            <p className="text-xs text-slate-500">#{log.id}</p>
          </div>
        </div>

        {/* VEHICLE INFO */}
        <div className="grid grid-cols-2 gap-4 text-sm mb-5">
          <div>
            <p><b>Owner:</b> {log.vehicle?.owner_name}</p>
            <p><b>RFID:</b> {log.vehicle?.rfid_id}</p>
          </div>
          <div>
            <p><b>Contact:</b> {log.vehicle?.owner_contact}</p>
            <p><b>Plate:</b> {log.vehicle?.plate}</p>
          </div>
        </div>

        {/* TIMES */}
        <div className="border rounded-lg text-sm mb-5">
          <div className="flex justify-between p-3 border-b">
            <span>Entrance</span>
            <span>{entrance.toLocaleString()}</span>
          </div>
          <div className="flex justify-between p-3 border-b">
            <span>Exit</span>
            <span>{exit.toLocaleString()}</span>
          </div>
          <div className="flex justify-between p-3">
            <span>Duration</span>
            <span>{hours}h {minutes}m</span>
          </div>
        </div>

        {/* TOTAL */}
        <div className="flex justify-between items-center text-lg font-bold border-t pt-4">
          <span>Total Amount</span>
          <span>{log.price?.toLocaleString()} تومان</span>
        </div>

        {/* FOOTER */}
        <div className="mt-6 text-center text-xs text-slate-500">
          Thank you for choosing <b>Slotify</b>
        </div>

        {/* ACTIONS */}
        <div className="mt-4 flex justify-end gap-3">
          <button
            onClick={onClose}
            className="px-4 py-2 text-sm rounded-lg border"
          >
            Close
          </button>
        </div>
      </div>
    </div>
  );
};

export default InvoiceModal;
