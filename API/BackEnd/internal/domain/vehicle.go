package domain

import "time"

type Vehicle struct {
	ID           uint      `gorm:"primaryKey" json:"id"`
	OwnerName    string    `json:"owner_name"`
	OwnerContact string    `json:"owner_contact"`
	Plate        string    `gorm:"unique" json:"plate"`
	RFIDID       string    `gorm:"unique" json:"rfid_id"`
	VehicleType  string    `json:"vehicle_type"`
	CreatedAt    time.Time `json:"created_at"`
}
