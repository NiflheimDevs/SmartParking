package main

import (
	"log"

	"github.com/niflheimdevs/smartparking/internal/db"
	"github.com/niflheimdevs/smartparking/internal/delivery/mqtt"
	"github.com/niflheimdevs/smartparking/internal/di"
)

func main() {
	app, err := di.InitializeApp()
	if err != nil {
		log.Fatalf("failed to initialize app: %v", err)
	}

	db.Migrate(app.DB)
	mqtt := mqtt.InitMQTT(app.Config)
	go mqtt.Listen()

	log.Println("🚀 Smart Parking System running on", app.Config.Server.Port)
	if err := app.Run(); err != nil {
		log.Fatal("server stopped:", err)
	}
}
