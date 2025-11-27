import { useState } from "react";
import CrudMenu from "./CrudMenu";
import CrudForm from "./CrudForm";

const CrudPanel = () => {
  const [action, setAction] = useState<"create" | "read" | "update" | "delete">("create");
  const [result, setResult] = useState<any>(null);
  const [debug, setDebug] = useState<string>("API responses will appear here...");

  // GLOBAL MODAL STATE
  const [showDeleteModal, setShowDeleteModal] = useState(false);
  const [pendingDeleteId, setPendingDeleteId] = useState<string | number>("");

  return (
    <>
      {/* GLOBAL MODAL - CENTER OF ENTIRE PAGE */}
      {showDeleteModal && (
        <div className="fixed inset-0 z-[9999] flex items-center justify-center bg-black/70 backdrop-blur-md">
          <div className="bg-slate-900/80 border border-white/10 shadow-xl rounded-xl p-6 w-[340px] text-center">
            
            <h2 className="text-cyan-400 font-semibold text-lg mb-3">
              Are you sure?
            </h2>

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
                }}
                className="px-4 py-2 rounded-lg bg-red-500 hover:bg-red-600 text-white"
              >
                Delete
              </button>
            </div>
          </div>
        </div>
      )}

      {/* MAIN GRID */}
      <div className="grid gap-4 sm:grid-cols-2 lg:grid-cols-4">

        {/* CRUD MENU */}
        <div className="panel p-6">
          <h3 className="text-cyan-400 font-semibold mb-3">CRUD Actions</h3>
          <CrudMenu action={action} onSelect={setAction} />
        </div>

        {/* FORM */}
        <div className="panel p-6">
          <CrudForm
            action={action}
            onResult={setResult}
            onDebug={setDebug}
            openDeleteModal={(id: string | number) => {
              setPendingDeleteId(id);
              setShowDeleteModal(true);
            }}
          />
        </div>

        {/* DATA */}
        <div className="panel p-6">
          <h3 className="text-cyan-400 font-semibold mb-3">Data Display</h3>
          <div className="overflow-auto text-sm whitespace-pre-wrap">
            {result ? <pre>{JSON.stringify(result, null, 2)}</pre> : "Results will appear here..."}
          </div>
        </div>

        {/* DEBUG */}
        <div className="panel p-6">
          <h3 className="text-cyan-400 font-semibold mb-3">Debug / Response</h3>
          <pre className="text-sm bg-slate-900/70 p-3 rounded-md overflow-x-auto border border-white/10">
            {debug}
          </pre>
        </div>

      </div>
    </>
  );
};

export default CrudPanel;
