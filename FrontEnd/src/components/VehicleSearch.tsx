// VehicleSearch.tsx
import { useState } from "react";
import { getVehicle } from "../api";

const VehicleSearch = () => {
  const [query, setQuery] = useState("");
  const [result, setResult] = useState<any>(null);
  const [loading, setLoading] = useState(false);

  const handleSearch = async () => {
    if (!query.trim()) return;
    setLoading(true);
    try {
      const res = await getVehicle(query);
      setResult(res.data);
    } catch (err) {
      setResult({ error: "Vehicle not found or API error" });
    } finally {
      setLoading(false);
    }
  };

  return (
    <section className="grid gap-4 lg:grid-cols-3">
      {/*  left : search bar  */}
      <div className="panel p-6 lg:col-span-1">
        <h3 className="text-accent font-semibold mb-4">Search Vehicle</h3>

        <div className="flex gap-2">
          <input
            value={query}
            onChange={(e) => setQuery(e.target.value)}
            placeholder="Enter Vehicle ID or RFID Tag"
            className="input"
          />
          <button onClick={handleSearch} disabled={loading} className="btn-accent">
            {loading ? "…" : "Search"}
          </button>
        </div>
      </div>

      {/*  right : result card  */}
      <div className="panel p-6 lg:col-span-2">
        <h3 className="text-accent font-semibold mb-4">Result</h3>

        {!result && (
          <p className="text-sm text-slate-400">Results will appear here…</p>
        )}

        {result && (
          <div className="bg-slate-900/60 rounded-md p-4 border-l-4 border-accent overflow-x-auto">
            <pre className="text-sm text-slate-100">{JSON.stringify(result, null, 2)}</pre>
          </div>
        )}
      </div>
    </section>
  );
};

export default VehicleSearch;