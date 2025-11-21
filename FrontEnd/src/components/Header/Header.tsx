// Header.tsx
"use client"; // if using Next.js
import React from "react";
import { motion } from "framer-motion";
import { Menu, X } from "lucide-react";

const navItems = ["Home", "Ecosystem", "Docs", "Community", "Launch App"];

export default function Header() {
  const [open, setOpen] = React.useState(false);

  return (
    <header className="fixed top-0 left-0 w-full z-50 bg-black/30 backdrop-blur-xl border-b border-white/10">
      <div className="max-w-7xl mx-auto flex justify-between items-center px-6 py-4">
        {/* Logo */}
        <motion.div
          initial={{ opacity: 0, y: -20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.6 }}
          className="text-2xl font-bold bg-gradient-to-r from-purple-400 via-pink-500 to-red-500 bg-clip-text text-transparent"
        >
          HatomClone
        </motion.div>

        {/* Desktop Nav */}
        <nav className="hidden md:flex gap-8 items-center">
          {navItems.map((item, i) => (
            <motion.a
              key={item}
              href="#"
              className="text-white/80 hover:text-white transition text-sm font-medium"
              initial={{ opacity: 0, y: -10 }}
              animate={{ opacity: 1, y: 0 }}
              transition={{ delay: i * 0.1 }}
            >
              {item}
            </motion.a>
          ))}
        </nav>

        {/* Mobile Button */}
        <button
          className="md:hidden text-white"
          onClick={() => setOpen(!open)}
        >
          {open ? <X size={28} /> : <Menu size={28} />}
        </button>
      </div>

      {/* Mobile Menu */}
      {open && (
        <motion.div
          initial={{ opacity: 0, y: -30 }}
          animate={{ opacity: 1, y: 0 }}
          className="absolute top-full left-0 w-full bg-black/90 backdrop-blur-xl flex flex-col items-center py-6 space-y-4 md:hidden"
        >
          {navItems.map((item) => (
            <a
              key={item}
              href="#"
              className="text-white/90 hover:text-white text-lg font-medium"
            >
              {item}
            </a>
          ))}
        </motion.div>
      )}
    </header>
  );
}
