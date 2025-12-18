import { toast } from "react-toastify";

export default function AccessDenied() {
  const handleAppeal = () => {
    toast.info("Bribe the CEO with pizza to get unbanned!", {
      position: "top-right",
      autoClose: 4000,
      hideProgressBar: false,
      closeOnClick: true,
      pauseOnHover: true,
      draggable: true,
      theme: "dark",
    });
  };

  return (
    <div className="min-h-screen flex items-center justify-center bg-gradient-to-br from-[#0f172a] to-[#1e293b] p-4">
      <div className="w-full max-w-2xl bg-slate-900/70 backdrop-blur-xl border border-slate-800 rounded-2xl shadow-2xl p-8 text-center">
        <h1 className="text-6xl font-extrabold text-red-500 mb-6 animate-pulse">403</h1>
        <h2 className="text-3xl font-bold text-yellow-400 mb-4">Banned Forever 😈</h2>
        <p className="text-slate-400 mb-6 text-lg">
          Congratulations! You've officially achieved "Most Unwanted User" status. 🎉  
          No cookies, no access, and definitely no sympathy.
        </p>
        <p className="text-slate-500 mb-6 text-sm">
          Maybe try being nice next time… or don’t. Your choice. 🤷‍♂️
        </p>

        <button
          onClick={handleAppeal}
          className="px-6 py-3 bg-red-600 hover:bg-red-700 text-white font-bold rounded-xl shadow-lg transition"
        >
          Appeal for Unban
        </button>
      </div>
    </div>
  );
}
