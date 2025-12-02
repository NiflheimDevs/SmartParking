import { useState } from "react";
import Navbar from "./components/Navbar";
import CrudPanel from "./components/CRUD/CrudPanel";
import VehicleSearch from "./components/VehicleSearch";
import Schematic from "./components/Schematic";
import EntranceExitPanel from "./components/EntranceExitPanel";
import Login from "./components/Auth/Login";
import Signup from "./components/Auth/Signup";

const isAuthenticated = () => {
  const token = localStorage.getItem("authToken");
  return !!token;
}

function App() {
  const [authMode, setAuthMode] = useState("login"); // "login" | "signup"

  type Tab = "crud" | "search" | "schematic" | "logs";
  const [activeTab, setActiveTab] = useState<Tab>("crud");

  if (!isAuthenticated()) {
    return (
      <div className="min-h-screen flex items-center justify-center bg-slate-900 text-slate-100">
        {authMode === "login" ? (
          <Login switchToSignup={() => setAuthMode("signup")} />
        ) : (
          <Signup switchToLogin={() => setAuthMode("login")} />
        )}
      </div>
    );
  }

  return (
    <div className="min-h-screen bg-gradient-to-br from-[#0f172a] to-[#1e293b] text-slate-100 pt-16">
      <Navbar active={activeTab} onChange={setActiveTab} />
      <div className="max-w-6xl mx-auto px-4 py-6">
        {activeTab === "crud" && <CrudPanel />}
        {activeTab === "search" && <VehicleSearch />}
        {activeTab === "schematic" && <Schematic />}
        {activeTab === "logs" && <EntranceExitPanel />}
      </div>
    </div>
  );
}

export default App;
