import { useState, useEffect } from "react";
import { getAllLogs, getLogById, getLogsByVehicle } from "../../api";
import LogCard from "./LogCard"

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
    <section className="grid gap-6 lg:grid-cols-3">

      {/* LEFT PANEL */}
      <div className="panel p-6 lg:col-span-1">
        <h3 className="text-accent font-semibold mb-4 text-lg">
          Entrance / Exit Logs
        </h3>

        {/* Mode Buttons */}
        <div className="flex flex-col gap-3">
          <button
            className={`btn-accent py-2 ${mode === "all" ? "" : "opacity-50 hover:opacity-70"}`}
            onClick={() => setMode("all")}
          >
            Show All Logs
          </button>

          <button
            className={`btn-accent py-2 ${mode === "log" ? "" : "opacity-50 hover:opacity-70"}`}
            onClick={() => setMode("log")}
          >
            Search by Log ID
          </button>

          <button
            className={`btn-accent py-2 ${mode === "vehicle" ? "" : "opacity-50 hover:opacity-70"}`}
            onClick={() => setMode("vehicle")}
          >
            Search by Vehicle ID
          </button>
        </div>

        {/* SEARCH FIELD */}
        {mode !== "all" && (
          <div className="flex gap-2 mt-2">
            <input
              value={query}
              onChange={(e) => setQuery(e.target.value)}
              placeholder={
                mode === "log"
                  ? "Enter Log ID (e.g., 98237)"
                  : "Enter Vehicle ID (e.g., 21A-445)"
              }
              className="input flex-1"
            />
            <button
              className="btn-accent px-4"
              onClick={handleSearch}
            >
              Go
            </button>
          </div>
        )}
      </div>

      {/* RIGHT PANEL */}
      <div className="panel p-6 lg:col-span-2">
        <div className="flex items-center justify-between mb-4">
          <h3 className="text-accent font-semibold text-lg">
            {loading ? "Loading…" : `Results (${logs.length})`}
          </h3>
        </div>

        {loading && (
          <p className="text-slate-400 text-sm animate-pulse">Loading logs…</p>
        )}

        {!loading && logs.length === 0 && (
          <p className="text-slate-400 text-sm">
            No logs found for the current search.
          </p>
        )}

        {!loading && logs.length > 0 && (
          <div className="grid gap-4 max-h-[525px] overflow-auto pr-2">
            {logs.map((log) => (
              <LogCard key={log.id} log={log} />
            ))}
          </div>
        )}
      </div>
    </section>
  );
};

export default EntranceExitPanel;
