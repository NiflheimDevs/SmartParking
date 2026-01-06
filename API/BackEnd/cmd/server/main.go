package main

import (
	"log"

	"github.com/niflheimdevs/smartparking/internal/db"
	"github.com/niflheimdevs/smartparking/internal/di"
)

func main() {
	app, err := di.InitializeApp()
	if err != nil {
		log.Fatalf("failed to initialize app: %v", err)
	}

	db.Migrate(app.PGDB)

	go app.MQTTClient.Listen()

	log.Println("🚀 Smart Parking System running on", app.HttpApp.Config.Server.Port)
	if err := app.HttpApp.Run(); err != nil {
		log.Fatal("server stopped:", err)
	}
}
