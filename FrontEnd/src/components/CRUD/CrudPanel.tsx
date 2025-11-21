import { useState } from "react";
import CrudMenu from "./CrudMenu";
import CrudForm from "./CrudForm";

const CrudPanel = () => {
  const [action, setAction] = useState<"create" | "read" | "update" | "delete">("create");
  const [result, setResult] = useState<any>(null);
  const [debug, setDebug] = useState<string>("API responses will appear here...");

  return (
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
        />
      </div>

      {/* DATA DISPLAY */}
      <div className="panel p-6">
        <h3 className="text-cyan-400 font-semibold mb-3">Data Display</h3>
        <div className="overflow-auto text-sm whitespace-pre-wrap">
          {result ? <pre>{JSON.stringify(result, null, 2)}</pre> : "Results will appear here..."}
        </div>
      </div>

      {/* DEBUG PANEL */}
      <div className="panel p-6">
        <h3 className="text-cyan-400 font-semibold mb-3">Debug / Response</h3>
        <pre className="text-sm bg-slate-900/70 p-3 rounded-md overflow-x-auto border border-white/10">
          {debug}
        </pre>
      </div>
    </div>
  );
};

export default CrudPanel;
