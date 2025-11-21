import { useState } from "react";
import Navbar from "./components/Navbar";
import CrudPanel from "./components/CRUD/CrudPanel";
import VehicleSearch from "./components/VehicleSearch";
import Schematic from "./components/Schematic";

function App() {
  type Tab = "crud" | "search" | "schematic";
  const [activeTab, setActiveTab] = useState<Tab>("crud");

  return (
    <div className="min-h-screen bg-gradient-to-br from-[#0f172a] to-[#1e293b] text-slate-100 pt-16">
      <Navbar active={activeTab} onChange={setActiveTab} />
      <div className="max-w-6xl mx-auto px-4 py-6">
        {activeTab === "crud" && <CrudPanel />}
        {activeTab === "search" && <VehicleSearch />}
        {activeTab === "schematic" && <Schematic />}
      </div>
    </div>
  );
}

export default App;
