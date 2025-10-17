package db

import (
	"github.com/niflheimdevs/smartparking/internal/domain"
	"gorm.io/gorm"
)

func Migrate(db *gorm.DB) {
	db.AutoMigrate(
		&domain.Vehicle{},
		&domain.ParkingSpot{},
		&domain.EntranceExit{},
	)
}
