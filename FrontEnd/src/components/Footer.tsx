const Footer = () => {
  return (
    <footer className="w-full flex justify-center mt-8">
      <div className="bg-slate-800/50 backdrop-blur-md border border-slate-700 rounded-2xl shadow-lg py-4 px-6 max-w-md w-full text-center">
        <p className="text-slate-300 text-sm">
          Made with <span className="text-red-500">❤️</span> at IUST by{" "}
          <a
            className="text-accent font-semibold hover:underline"
            rel="noopener noreferrer"
            href="https://github.com/niflheimdevs"
            target="_blank"
          >
            Niflheimdevs Team
          </a>
        </p>
        <p className="text-slate-400 text-xs mt-1">
          &copy; {new Date().getFullYear()} All rights reserved.
        </p>
      </div>
    </footer>
  );
};

export default Footer;
