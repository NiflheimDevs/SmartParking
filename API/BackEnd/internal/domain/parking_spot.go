package domain

import "time"

type ParkingSpot struct {
	ID          uint      `gorm:"primaryKey;autoIncrement" json:"id"`
	SpotNumber  string    `gorm:"type:varchar(20);unique;not null" json:"spot_number"`
	IsOccupied  bool      `gorm:"default:false" json:"is_occupied"`
	LastUpdated time.Time `gorm:"autoUpdateTime" json:"last_updated"`

	EntranceExits []EntranceExit `gorm:"foreignKey:SpotID" json:"entrance_exits,omitempty"`
}
