# Smart Parking System

A comprehensive IoT-based smart parking solution that enables real-time parking spot management, vehicle tracking, and entrance/exit monitoring. The system integrates hardware sensors, cloud backend services, and a modern web interface to optimize parking operations.

---

## 📋 Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Tech Stack](#tech-stack)
- [How to Run](#how-to-run)
- [Folder Structure](#folder-structure)
- [API Documentation](#api-documentation)
- [Contributing](#contributing)
- [Contributors](#-contributors)

---

## 🎯 Project Overview

Smart Parking System is a full-stack IoT application designed to streamline parking management. It features:

- **Real-time parking spot occupancy tracking** using IoT sensors
- **User authentication and authorization** with JWT tokens
- **MQTT-based communication** between hardware devices and backend
- **RESTful API** for vehicle and parking management
- **Interactive web dashboard** for users and administrators
- **RFID integration** for vehicle identification

The system consists of three main components: Backend API (Go), Frontend (React + TypeScript), and Hardware (ESP32).

---

## ✨ Features

### Backend Features
- ✅ User authentication and registration with secure password hashing
- ✅ Real-time parking spot status monitoring
- ✅ Vehicle management and tracking
- ✅ Entrance/Exit logging and auditing
- ✅ User ban/unban functionality with IP logging
- ✅ JWT-based authorization

### Hardware Features
- 📡 MQTT connectivity for IoT devices
- 🔐 RFID reader integration for vehicle identification
- 🎮 Servo motor control for barrier management
- 💡 LED indicators for status visualization
- 📏 Ultrasonic sensors for occupancy detection
- 📺 LCD display for user feedback

### Frontend Features
- 🎨 Modern, responsive UI with Tailwind CSS
- 🔐 Secure login and signup pages
- 📊 Parking spot schematic visualization
- 🚗 Vehicle search and management
- 📝 Entrance/exit logging dashboard
- 🎮 Interactive mini-games for user engagement
- 🎬 Smooth animations with Framer Motion

---

## 🛠 Tech Stack

### Backend
- **Language**: Go 1.24.4
- **Framework**: Gin Web Framework
- **Database**: PostgreSQL
- **Cache**: Redis
- **Message Broker**: EMQX (MQTT)
- **Authentication**: JWT (golang-jwt/jwt)
- **Encryption**: golang.org/x/crypto
- **ORM**: GORM
- **Dependency Injection**: Google Wire

### Frontend
- **Framework**: React 19
- **Language**: TypeScript 5.8
- **Build Tool**: Vite 7
- **Styling**: Tailwind CSS 4
- **HTTP Client**: Axios
- **Animation**: Framer Motion
- **Icons**: Lucide React
- **Notifications**: React Toastify
- **Linting**: ESLint

### Hardware
- **Platform**: ESP32 / ESP32-S3
- **Framework**: Arduino
- **Development**: PlatformIO
- **Libraries**: Arduino JSON, MQTT 2.5.2

### Infrastructure
- **Containerization**: Docker & Docker Compose
- **Web Server**: Caddy
- **Package Registry**: GitHub Container Registry (ghcr.io)

---

## 🚀 How to Run

### Option 1: Docker Compose (Recommended)

```bash
cd API
docker-compose up -d
```

This will start:
- Backend API on `http://localhost:8080`
- EMQX Broker on `mqtt://localhost:1883`
- EMQX Dashboard on `http://localhost:18083`
- PostgreSQL on `localhost:5432`
- Redis on `localhost:6379`

```bash
cd Frontend
docker-compose up -d
```

This will start:
- Frontend Web Server on `http://localhost:3000`

### Option 2: Manual Setup

#### Backend

```bash
cd API/BackEnd
go run cmd/server/main.go
```

#### Frontend

```bash
cd FrontEnd
npm run dev
```

Frontend will be available at `http://localhost:5173`

#### Build for Production

**Backend:**
```bash
cd API/BackEnd
go build -o smartparking cmd/server/main.go
./smartparking
```

**Frontend:**
```bash
cd FrontEnd
npm run build
npm run preview
```

### Option 3: Docker (Individual Services)

**Backend Only:**
```bash
cd API/BackEnd
docker build -t smartparking:latest .
docker run -p 8080:8080 smartparking:latest
```

**Frontend Only:**
```bash
cd FrontEnd
docker build -t smartparking-frontend:latest .
docker run -p 3000:3000 smartparking-frontend:latest
```

---

## 📁 Folder Structure

```
smart-parking/
├── README.md                          # Project documentation
├── API/                              # Backend & Infrastructure
│   ├── docker-compose.yml            # Docker services configuration
│   ├── BackEnd/                      # Go backend application
│   │   ├── dockerfile                # Backend container image
│   │   ├── go.mod                    # Go module definitions
│   │   ├── cmd/
│   │   │   └── server/
│   │   │       └── main.go           # Application entry point
│   │   └── internal/
│   │       ├── config/               # Configuration management
│   │       ├── db/                   # Database connection & migrations
│   │       │   └── migration/
│   │       │       └── init.sql      # Database schema
│   │       ├── delivery/
│   │       │   ├── http/             # HTTP handlers & routes
│   │       │   └── mqtt/             # MQTT client & handlers
│   │       ├── di/                   # Dependency injection setup
│   │       ├── domain/               # Business domain models
│   │       ├── dto/                  # Data transfer objects
│   │       ├── jwt/                  # JWT utilities
│   │       ├── middleware/           # HTTP middleware
│   │       ├── repository/           # Data access layer
│   │       │   ├── postgres/         # PostgreSQL repositories
│   │       │   └── redis/            # Redis repositories
│   │       └── usecase/              # Business logic layer
│   │
├── FrontEnd/                         # React frontend application
│   ├── dockerfile                    # Frontend container image
│   ├── docker-compose.yml            # Frontend services
│   ├── nginx.conf                    # Nginx configuration
│   ├── package.json                  # NPM dependencies
│   ├── vite.config.ts                # Vite build configuration
│   ├── tsconfig.json                 # TypeScript configuration
│   ├── index.html                    # HTML entry point
│   ├── public/                       # Static assets
│   │   ├── Chopping/                 # Chopping mini-game
│   │   └── RepairKit/                # RepairKit mini-game
│   └── src/
│       ├── main.tsx                  # React app entry point
│       ├── App.tsx                   # Root component
│       ├── api.ts                    # API client setup
│       ├── index.css                 # Global styles
│       └── components/
│           ├── Auth/                 # Login/Signup components
│           ├── CRUD/                 # CRUD operation components
│           ├── EntranceExit/         # Entry/exit logging components
│           ├── VehicleSearch/        # Vehicle search components
│           ├── minigame/             # Mini-game components
│           ├── Navbar.tsx            # Navigation bar
│           ├── Footer.tsx            # Footer component
│           ├── Schematic.tsx         # Parking spot visualization
│           └── AccessDenied.tsx      # Access denied page
│
├── Hardware/                         # ESP32 firmware
│   ├── platformio.ini                # PlatformIO configuration
│   ├── include/                      # Header files
│   │   ├── config.h                  # Configuration settings
│   │   ├── esp32.h / esp32_s3.h      # Platform definitions
│   │   ├── Led/                      # LED control headers
│   │   ├── motors/                   # Motor control headers
│   │   ├── MQTT/                     # MQTT client headers
│   │   ├── RFID/                     # RFID reader headers
│   │   └── sensors/                  # Sensor headers
│   ├── src/                          # Source code
│   │   ├── main.cpp                  # Firmware entry point
│   │   ├── esp32.cpp / esp32_s3.cpp  # Platform initialization
│   │   ├── Lcd/                      # LCD display implementation
│   │   ├── Led/                      # LED control implementation
│   │   ├── motors/                   # Motor control implementation
│   │   ├── MQTT/                     # MQTT implementation
│   │   ├── RFID/                     # RFID reader implementation
│   │   └── sensors/                  # Sensor implementations
│   └── test/                         # Test files
```

---

## 📡 API Documentation

### Authentication
- **POST** `/v1/signup` - Register a new user
- **POST** `/v1/login` - User login

### Parking Spots
- **GET** `/v1/ps` - Get all parking spots

### Vehicles
- **GET** `/v1/vehicles` - Get users vehicles
- **GET** `/v1/vehicles/:id` - Get vehicle info
- **POST** `/v1/vehicles` - Register a new vehicle
- **PUT** `/v1/vehicles/:id` - Update a vehicle info
- **DELETE** `/v1/vehicles/:id` - Delete vehicle

### Entrance/Exit
- **GET** `/v1/ee` - Get entry/exit logs
- **GET** `/v1/ee/:id` - Get entry/exit log info
- **GET** `/v1/ee/vehicle/:id` - Get vehicle entrance/exit log

### Gate Control
- **POST** `/v1/gate/control` - control entrance/exit gate


---

## 🤝 Contributing

1. Create a feature branch (`git checkout -b feature/AmazingFeature`)
2. Commit your changes (`git commit -m 'Add AmazingFeature'`)
3. Push to the branch (`git push origin feature/AmazingFeature`)
4. Open a Pull Request

---

## 🧑‍💻 Contributors

- [Saman SayadFaal](https://github.com/samansayad93)
- [Ali Gholami](https://github.com/alig41382)
- [Parsa Samiei](https://github.com/parsasamiei)

---

## 📄 License

This project is licensed under the MIT License.

Copyright (c) 2026 NiflheimDevs

See [LICENSE](LICENSE) for details.

---

## 👥 Support

For issues, questions, or suggestions, please create an issue in the repository or contact the development team.
