package main

import (
	"log"

	"github.com/niflheimdevs/smartparking/internal/db"
	"github.com/niflheimdevs/smartparking/internal/di"
	"github.com/niflheimdevs/smartparking/internal/logger"
)

func main() {
	logger.SetupLogger()

	http_app, err := di.InitializeHttpApp()
	if err != nil {
		log.Fatalf("failed to initialize app: %v", err)
	}

	db.Migrate(http_app.DB)

	mqtt_app, err := di.InitializeMQTTApp(http_app.Config, http_app.DB)
	if err != nil {
		log.Fatalf("failed to initialize app: %v", err)
	}

	go mqtt_app.Listen()

	log.Println("🚀 Smart Parking System running on", http_app.Config.Server.Port)
	if err := http_app.Run(); err != nil {
		log.Fatal("server stopped:", err)
	}
}
