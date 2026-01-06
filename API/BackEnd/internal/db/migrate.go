package db

import (
	"fmt"
	"log"

	"github.com/niflheimdevs/smartparking/internal/domain"
	"gorm.io/gorm"
)

func seedParkingSpot(db *gorm.DB) {
	var count int64
	db.Model(&domain.ParkingSpot{}).Count(&count)

	if count == 0 {
		log.Println("🚗 Seeding 7 parking spots...")
		spots := make([]domain.ParkingSpot, 8)
		spots[0] = domain.ParkingSpot{
			SpotNumber: "In the way",
			IsOccupied: false,
		}
		for i := 1; i < 8; i++ {
			spots[i] = domain.ParkingSpot{
				SpotNumber: fmt.Sprintf("P%d", i),
				IsOccupied: false,
			}
		}

		if err := db.Create(&spots).Error; err != nil {
			log.Printf("❌ Failed to seed parking spots: %v", err)
		} else {
			log.Println("✅ Parking spots seeded successfully.")
		}
	} else {
		log.Println("ℹ️ Parking spots already exist, skipping seed.")
	}
}

func Migrate(db *gorm.DB) {
	db.AutoMigrate(
		&domain.Vehicle{},
		&domain.ParkingSpot{},
		&domain.EntranceExit{},
		&domain.User{},
		&domain.BannedIP{},
	)
	seedParkingSpot(db)
}
