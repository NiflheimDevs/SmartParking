package domain

import "time"

type EntranceExit struct {
	ID           int       `gorm:"primaryKey" json:"id"`
	VehicleID    int       `gorm:"" json:"vehicle_id"`
	EntranceTime time.Time `json:"enterance_time"`
	ExitTime     time.Time `json:"exit_time"`
	SpotID       int       `json:"spot_id"`
}
