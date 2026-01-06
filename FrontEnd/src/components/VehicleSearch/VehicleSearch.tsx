import { useEffect, useState } from "react";
import { getAllVehicles } from "../../api";
import VehicleCard from "./VehicleCard";

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
    <section className="grid gap-6 lg:grid-cols-3">

      {/* LEFT SEARCH PANEL */}
      <div className="panel p-6 lg:col-span-1">
        <h3 className="text-accent font-semibold text-lg mb-2">
          Vehicle Search
        </h3>

        <div className="flex gap-2 mt-2">
          <input
            value={query}
            onChange={(e) => setQuery(e.target.value)}
            placeholder="Search plate, RFID, owner..."
            className="input flex-1"
          />
          <button
            onClick={handleSearch}
            disabled={loading}
            className="btn-accent px-4"
          >
            {loading ? "…" : "Go"}
          </button>
        </div>
      </div>

      {/* RIGHT RESULTS PANEL */}
      <div className="panel p-6 lg:col-span-2">
        <div className="flex items-center justify-between mb-4">
          <h3 className="text-accent font-semibold text-lg">
            {loading ? "Loading…" : `Results (${filtered.length})`}
          </h3>
        </div>

        {/* Loading */}
        {loading && (
          <p className="text-slate-400 text-sm animate-pulse">
            Loading vehicles…
          </p>
        )}

        {/* No Results */}
        {!loading && filtered.length === 0 && (
          <p className="text-slate-400 text-sm">
            No vehicles found.
          </p>
        )}

        {/* Card Results */}
        {!loading && filtered.length > 0 && (
          <div className="grid gap-4 max-h-[525px] overflow-auto pr-2">
            {filtered.map((v) => (
              <VehicleCard key={v.id} vehicle={v} />
            ))}
          </div>
        )}
      </div>
    </section>
  );
};

export default VehicleSearch;
