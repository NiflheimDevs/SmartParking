// VehicleSearch.tsx
import { useEffect, useState } from "react";
import { getAllVehicles } from "../api";

const VehicleSearch = () => {
  const [query, setQuery] = useState("");
  const [allVehicles, setAllVehicles] = useState<any[]>([]);
  const [filtered, setFiltered] = useState<any[]>([]);
  const [loading, setLoading] = useState(true);

  // Load all vehicles on mount
  useEffect(() => {
    const load = async () => {
      try {
        const data = await getAllVehicles(); // API returns array directly
        setAllVehicles(data);
        setFiltered(data);
      } catch (err) {
        console.error("Error loading vehicles:", err);
      } finally {
        setLoading(false);
      }
    };

    load();
  }, []);

  // Search logic
  const handleSearch = () => {
    const q = query.trim().toLowerCase();

    if (!q) {
      setFiltered(allVehicles);
      return;
    }

    const results = allVehicles.filter((v) =>
      JSON.stringify(v).toLowerCase().includes(q)
    );

    setFiltered(results);
  };

  return (
    <section className="grid gap-4 lg:grid-cols-3">

      {/* SEARCH PANEL */}
      <div className="panel p-6 lg:col-span-1">
        <h3 className="text-accent font-semibold mb-4">Search Vehicles</h3>

        <div className="flex gap-2">
          <input
            value={query}
            onChange={(e) => setQuery(e.target.value)}
            placeholder="Search by plate, RFID, owner..."
            className="input"
          />
          <button
            onClick={handleSearch}
            disabled={loading}
            className="btn-accent"
          >
            {loading ? "…" : "Search"}
          </button>
        </div>
      </div>

      {/* RESULTS PANEL */}
      <div className="panel p-6 lg:col-span-2">
        <h3 className="text-accent font-semibold mb-4">
          {loading ? "Loading…" : `Results (${filtered.length})`}
        </h3>

        {/* Loading State */}
        {loading && (
          <p className="text-slate-400 text-sm">Loading vehicles…</p>
        )}

        {/* No Results */}
        {!loading && filtered.length === 0 && (
          <p className="text-slate-400 text-sm">No vehicles found.</p>
        )}

        {/* Results */}
        {!loading && filtered.length > 0 && (
          <div className="bg-slate-900/60 rounded-md p-4 border-l-4 border-accent max-h-[600px] overflow-auto">
            <pre className="text-sm text-slate-100">
              {JSON.stringify(filtered, null, 2)}
            </pre>
          </div>
        )}
      </div>
    </section>
  );
};

export default VehicleSearch;
