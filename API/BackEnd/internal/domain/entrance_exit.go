package domain

import "time"

type EntranceExit struct {
	ID           uint      `gorm:"primaryKey;autoIncrement" json:"id"`
	VehicleID    uint      `gorm:"not null" json:"vehicle_id"`
	SpotID       uint      `json:"spot_id,omitempty"`
	Price        int64     `gorm:"default:0" json:"price"`
	EntranceTime time.Time `gorm:"autoCreateTime" json:"entrance_time"`
	ExitTime     time.Time `json:"exit_time"`

	Vehicle     Vehicle      `gorm:"foreignKey:VehicleID;constraint:OnUpdate:CASCADE,OnDelete:CASCADE" json:"vehicle,omitempty"`
	ParkingSpot *ParkingSpot `gorm:"foreignKey:SpotID;constraint:OnUpdate:CASCADE,OnDelete:SET NULL" json:"parking_spot,omitempty"`
}
