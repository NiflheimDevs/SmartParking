import { useState } from "react";
import CrudMenu from "./CrudMenu";
import CrudForm from "./CrudForm";
import ResultCard from "./ResultCard";

const CrudPanel = () => {
  const [action, setAction] = useState<"create" | "read" | "update" | "delete">("create");
  const [result, setResult] = useState<any>(null);

  // Delete modal state
  const [showDeleteModal, setShowDeleteModal] = useState(false);
  const [pendingDeleteId, setPendingDeleteId] = useState<string | number>("");

  return (
    <>
      {/* DELETE MODAL */}
      {showDeleteModal && (
        <div className="fixed inset-0 z-[9999] flex items-center justify-center bg-black/70 backdrop-blur-md">
          <div className="bg-slate-900/80 border border-white/10 shadow-xl rounded-xl p-6 w-[340px] text-center">
            <h2 className="text-cyan-400 font-semibold text-lg mb-3">Are you sure?</h2>
            <p className="text-slate-300 text-sm mb-6">
              This action will permanently delete this vehicle.
            </p>
            <div className="flex justify-center gap-3">
              <button
                onClick={() => setShowDeleteModal(false)}
                className="px-4 py-2 rounded-lg bg-white/10 hover:bg-white/20 text-slate-200"
              >
                Cancel
              </button>
              <button
                onClick={() => {
                  document.dispatchEvent(
                    new CustomEvent("confirmDeleteVehicle", {
                      detail: { id: pendingDeleteId }
                    })
                  );
                  setShowDeleteModal(false);
                  // Show simple success message in result panel
                  setResult({ message: `Vehicle #${pendingDeleteId} deleted successfully.` });
                }}
                className="px-4 py-2 rounded-lg bg-red-500 hover:bg-red-600 text-white"
              >
                Delete
              </button>
            </div>
          </div>
        </div>
      )}

      {/* MAIN PANEL */}
      <div className="flex flex-col gap-4">
        {/* TOP ROW - Pill Buttons */}
        <div className="panel p-6 flex justify-center">
          <CrudMenu action={action} onSelect={setAction} />
        </div>

        {/* CONTENT ROW */}
        <div className="grid gap-4 lg:grid-cols-2">
          {/* LEFT COLUMN - FORM */}
          <div className="panel p-6 bg-slate-900/70 border border-slate-700 rounded-xl shadow-md">
            <CrudForm
              action={action}
              onResult={setResult}
              openDeleteModal={(id: string | number) => {
                setPendingDeleteId(id);
                setShowDeleteModal(true);
              }}
            />
          </div>

          {/* RIGHT COLUMN - RESULT */}
          <div className="panel p-6 bg-slate-900/70 border border-slate-700 rounded-xl shadow-md max-h-[650px] overflow-auto">
            <h3 className="text-cyan-400 font-semibold mb-3">Result</h3>

            {result ? (
              // If result has "message" (delete), show simple success box
              result.message ? (
                <div className="border border-green-600 bg-green-700/30 text-green-100 p-4 rounded-xl">
                  {result.message}
                </div>
              ) : (
                <ResultCard data={result} />
              )
            ) : (
              <p className="text-slate-400 text-sm">Results will appear here...</p>
            )}
          </div>
        </div>
      </div>
    </>
  );
};

export default CrudPanel;
