interface Props {
  action: "create" | "read" | "update" | "delete";
  onSelect: (a: "create" | "read" | "update" | "delete") => void;
}

const CrudMenu = ({ action, onSelect }: Props) => {
  const actions: Props["action"][] = ["create", "read", "update", "delete"];

  return (
    <div className="flex gap-3 flex-wrap">
      {actions.map((a) => (
        <button
          key={a}
          onClick={() => onSelect(a)}
          className={`px-4 py-2 rounded-full font-semibold transition-all duration-200
            ${
              action === a
                ? "bg-cyan-400 text-slate-900 shadow-lg"
                : "bg-cyan-400/20 text-cyan-300 hover:bg-cyan-400/30 hover:text-slate-50"
            }`}
        >
          {a.charAt(0).toUpperCase() + a.slice(1)}
        </button>
      ))}
    </div>
  );
};

export default CrudMenu;
