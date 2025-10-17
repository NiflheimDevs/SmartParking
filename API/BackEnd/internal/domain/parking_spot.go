package domain

import "time"

type ParkingSpot struct {
	ID         int       `gorm:"primaryKey" json:"id"`
	SpotNumber string    `gorm:"unique" json:"spot_number"`
	IsOccupied bool      `json:"is_occupied"`
	LastUpdate time.Time `json:"last_update"`
}
