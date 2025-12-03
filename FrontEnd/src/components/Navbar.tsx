interface Props {
  active: string;
  onChange: (tab: "crud" | "search" | "schematic" | "logs") => void;
  onLogout: () => void;
}

const Navbar = ({ active, onChange, onLogout }: Props) => {
  const tabs = [
    { id: "crud", label: "CRUD" },
    { id: "search", label: "Vehicle Search" },
    { id: "schematic", label: "Schematic" },
    { id: "logs", label: "Entrance / Exit" },
  ];

  return (
    <nav
      className="fixed top-0 left-0 right-0 h-14 flex items-center px-6 z-50
                bg-slate-900/60 backdrop-blur-md border-b border-white/10 shadow-lg"
    >
      <div className="mr-6 flex items-center">
        <img
          src="/logo-transparent.png"
          alt="Logo"
          className="h-10 w-auto object-contain"
        />
      </div>

      {tabs.map((tab) => (
        <button
          key={tab.id}
          onClick={() => onChange(tab.id as any)}
          className={`px-4 py-2 mr-2 rounded-lg text-sm font-semibold transition-all hover: cursor-pointer
                  ${
                    active === tab.id
                      ? "bg-[#00c6ff] text-slate-900 shadow-[0_0_15px_var(--accent)]"
                      : "text-slate-200 hover:bg-white/10"
                  }`}
        >
          {tab.label}
        </button>
      ))}

      <button
        onClick={onLogout}
        className="ml-auto px-4 py-2 rounded-lg text-sm font-semibold text-red-400 hover:bg-red-500/20 transition-all"
      >
        Logout
      </button>
    </nav>
  );
};

export default Navbar;