interface Props {
  action: "create" | "read" | "update" | "delete";
  onSelect: (a: "create" | "read" | "update" | "delete") => void;
}

const CrudMenu = ({ action, onSelect }: Props) => {
  const actions = ["create", "read", "update", "delete"];
  return (
    <div className="flex flex-col gap-2">
      {actions.map((a) => (
        <button
          key={a}
          onClick={() => onSelect(a as any)}
          className={`px-3 py-2 rounded-md text-left capitalize transition ${
            action === a
              ? "bg-cyan-400 text-slate-900 font-semibold"
              : "bg-cyan-400/10 border border-cyan-400/20 hover:bg-cyan-400/20"
          }`}
        >
          {a}
        </button>
      ))}
    </div>
  );
};

export default CrudMenu;
