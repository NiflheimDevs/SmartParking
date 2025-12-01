import { useState, useEffect } from "react";
import { getAllLogs, getLogById, getLogsByVehicle } from "../api";

const EntranceExitPanel = () => {
  const [logs, setLogs] = useState<any[]>([]);
  const [loading, setLoading] = useState(true);

  const [query, setQuery] = useState("");
  const [mode, setMode] = useState<"all" | "log" | "vehicle">("all");

  // LOAD ALL LOGS
  const loadAllLogs = async () => {
    try {
      setLoading(true);
      const data = await getAllLogs();
      setLogs(data);
    } catch (err) {
      console.error("Error loading logs:", err);
      setLogs([]);
    } finally {
      setLoading(false);
    }
  };

  // SEARCH based on mode
  const handleSearch = async () => {
    setLoading(true);

    try {
      if (mode === "all") {
        await loadAllLogs();
        return;
      }

      if (!query.trim()) return;

      if (mode === "log") {
        const data = await getLogById(query);
        setLogs([data]);
      }

      if (mode === "vehicle") {
        const data = await getLogsByVehicle(query);
        setLogs(data);
      }
    } catch (err) {
      console.error("Search error:", err);
      setLogs([]);
    }

    setLoading(false);
  };

  // 🔥 Re-fetch whenever mode changes
  useEffect(() => {
    if (mode === "all") {
      loadAllLogs();
    } else {
      // clear old results when switching to log / vehicle mode
      setLogs([]);
    }
  }, [mode]);

  // Initial load
  useEffect(() => {
    loadAllLogs();
  }, []);

  return (
    <section className="grid gap-4 lg:grid-cols-3">

      {/* LEFT PANEL */}
      <div className="panel p-6 lg:col-span-1">
        <h3 className="text-accent font-semibold mb-4">Entrance / Exit Logs</h3>

        {/* Mode Buttons */}
        <div className="flex flex-col gap-2 mb-4">
          <button
            className={`btn-accent ${mode === "all" ? "" : "opacity-40"}`}
            onClick={() => setMode("all")}
          >
            Show All Logs
          </button>

          <button
            className={`btn-accent ${mode === "log" ? "" : "opacity-40"}`}
            onClick={() => setMode("log")}
          >
            Search by Log ID
          </button>

          <button
            className={`btn-accent ${mode === "vehicle" ? "" : "opacity-40"}`}
            onClick={() => setMode("vehicle")}
          >
            Search by Vehicle ID
          </button>
        </div>

        {/* SEARCH FIELD */}
        {mode !== "all" && (
          <div className="flex gap-2">
            <input
              value={query}
              onChange={(e) => setQuery(e.target.value)}
              placeholder={mode === "log" ? "Enter Log ID" : "Enter Vehicle ID"}
              className="input"
            />
            <button className="btn-accent" onClick={handleSearch}>
              Search
            </button>
          </div>
        )}
      </div>

      {/* RIGHT PANEL */}
      <div className="panel p-6 lg:col-span-2">
        <h3 className="text-accent font-semibold mb-4">
          {loading ? "Loading…" : `Results (${logs.length})`}
        </h3>

        {loading && <p className="text-slate-400 text-sm">Loading logs…</p>}

        {!loading && logs.length === 0 && (
          <p className="text-slate-400 text-sm">No logs found.</p>
        )}

        {!loading && logs.length > 0 && (
          <div className="bg-slate-900/60 rounded-md p-4 border-l-4 border-accent max-h-[600px] overflow-auto">
            <pre className="text-sm text-slate-100">
              {JSON.stringify(logs, null, 2)}
            </pre>
          </div>
        )}
      </div>

    </section>
  );
};

export default EntranceExitPanel;
