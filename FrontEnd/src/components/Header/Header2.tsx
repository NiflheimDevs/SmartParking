// // src/components/Header.tsx
// import { useState, useEffect } from "react";
// import { motion, AnimatePresence } from "framer-motion";

// const navItems = [
//   { name: "Home", href: "#" },
//   { name: "Products", href: "#" },
//   { name: "Ecosystem", href: "#" },
//   { name: "Developers", href: "#" },
//   { name: "Contact", href: "#" },
// ];

// export default function Header() {
//   const [scrolled, setScrolled] = useState(false);
//   const [open, setOpen] = useState(false);

//   useEffect(() => {
//     const handleScroll = () => {
//       setScrolled(window.scrollY > 30);
//     };
//     window.addEventListener("scroll", handleScroll);
//     return () => window.removeEventListener("scroll", handleScroll);
//   }, []);

//   return (
//     <header
//       className={`fixed top-0 left-0 w-full z-50 transition-colors duration-300 ${
//         scrolled ? "backdrop-blur-xl bg-black/60 shadow-md" : "bg-transparent"
//       }`}
//     >
//       <div className="mx-auto flex max-w-7xl items-center justify-between px-6 py-4">
//         {/* Logo */}
//         <motion.a
//           href="#"
//           className="text-2xl font-bold text-white"
//           initial={{ opacity: 0, y: -20 }}
//           animate={{ opacity: 1, y: 0 }}
//           transition={{ duration: 0.5 }}
//         >
//           H<span className="text-blue-400">•</span>Clone
//         </motion.a>

//         {/* Desktop nav */}
//         <nav className="hidden md:flex gap-8 text-white">
//           {navItems.map((item) => (
//             <motion.a
//               key={item.name}
//               href={item.href}
//               className="relative group"
//               whileHover={{ y: -2 }}
//             >
//               {item.name}
//               <span className="absolute left-0 bottom-[-4px] h-[2px] w-0 bg-blue-400 transition-all group-hover:w-full" />
//             </motion.a>
//           ))}
//         </nav>

//         {/* Burger */}
//         <button
//           onClick={() => setOpen(!open)}
//           className="md:hidden relative z-50 flex h-8 w-8 flex-col justify-between"
//         >
//           <span
//             className={`h-[2px] w-full bg-white transition-transform ${
//               open ? "rotate-45 translate-y-3" : ""
//             }`}
//           />
//           <span
//             className={`h-[2px] w-full bg-white transition-opacity ${
//               open ? "opacity-0" : ""
//             }`}
//           />
//           <span
//             className={`h-[2px] w-full bg-white transition-transform ${
//               open ? "-rotate-45 -translate-y-3" : ""
//             }`}
//           />
//         </button>
//       </div>

//       {/* Mobile menu */}
//       <AnimatePresence>
//         {open && (
//           <motion.nav
//             initial={{ opacity: 0 }}
//             animate={{ opacity: 1 }}
//             exit={{ opacity: 0 }}
//             className="fixed inset-0 z-40 flex flex-col items-center justify-center bg-black/90 text-white space-y-8 text-2xl"
//           >
//             {navItems.map((item, i) => (
//               <motion.a
//                 key={item.name}
//                 href={item.href}
//                 initial={{ y: 20, opacity: 0 }}
//                 animate={{ y: 0, opacity: 1 }}
//                 transition={{ delay: i * 0.1 }}
//                 onClick={() => setOpen(false)}
//               >
//                 {item.name}
//               </motion.a>
//             ))}
//           </motion.nav>
//         )}
//       </AnimatePresence>
//     </header>
//   );
// }
