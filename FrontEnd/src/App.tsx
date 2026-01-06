import { useEffect, useState } from "react";
import Navbar from "./components/Navbar";
import CrudPanel from "./components/CRUD/CrudPanel";
import VehicleSearch from "./components/VehicleSearch/VehicleSearch";
import Schematic from "./components/Schematic";
import EntranceExitPanel from "./components/EntranceExit/EntranceExitPanel";
import Login from "./components/Auth/Login";
import Signup from "./components/Auth/Signup";
import Footer from "./components/Footer";
import AccessDenied from "./components/AccessDenied";
import { ToastContainer } from "react-toastify";
import "react-toastify/dist/ReactToastify.css";

const isAuthenticated = () => {
  const token = localStorage.getItem("authToken");
  return !!token;
};

type Tab = "crud" | "search" | "schematic" | "logs";

function App() {
  const [authMode, setAuthMode] = useState<"login" | "signup">("login");
  const [activeTab, setActiveTab] = useState<Tab>("crud");
  const [accessDenied, setAccessDenied] = useState(false);

  useEffect(() => {
    const handler = () => setAccessDenied(true);
    window.addEventListener("access-denied", handler);

    return () => {
      window.removeEventListener("access-denied", handler);
    };
  }, []);

  const handleLogout = () => {
    localStorage.removeItem("authToken");
    window.location.reload();
  };

  if (accessDenied) {
    return (
      <div>
        <AccessDenied />
        <ToastContainer
          position="top-right"
          autoClose={3000}
          hideProgressBar={false}
          newestOnTop
          closeOnClick
          pauseOnHover
          theme="dark"
        />
      </div>
    );
  }

  if (!isAuthenticated()) {
    return (
      <div>
        <div className="min-h-screen flex items-center justify-center bg-slate-900 text-slate-100">
          {authMode === "login" ? (
            <Login switchToSignup={() => setAuthMode("signup")} />
          ) : (
            <Signup switchToLogin={() => setAuthMode("login")} />
          )}
        </div>
        <ToastContainer
          position="top-right"
          autoClose={3000}
          hideProgressBar={false}
          newestOnTop
          closeOnClick
          pauseOnHover
          theme="dark"
        />
      </div>
    );
  }

  return (
    <div className="min-h-screen bg-gradient-to-br from-[#0f172a] to-[#1e293b] text-slate-100 pt-16">
      <Navbar
        active={activeTab}
        onChange={setActiveTab}
        onLogout={handleLogout}
      />

      <div className="max-w-6xl mx-auto px-4 py-6">
        {activeTab === "crud" && <CrudPanel />}
        {activeTab === "search" && <VehicleSearch />}
        {activeTab === "schematic" && <Schematic />}
        {activeTab === "logs" && <EntranceExitPanel />}
      </div>

      <Footer />
      <ToastContainer
        position="top-right"
        autoClose={3000}
        hideProgressBar={false}
        newestOnTop
        closeOnClick
        pauseOnHover
        theme="dark"
      />
    </div>
  );
}

export default App;
