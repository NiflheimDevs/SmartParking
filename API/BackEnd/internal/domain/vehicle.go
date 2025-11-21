package domain

import "time"

type Vehicle struct {
	ID           uint      `gorm:"primaryKey;autoIncrement" json:"id"`
	OwnerName    string    `gorm:"type:varchar(100);not null" json:"owner_name"`
	OwnerContact string    `gorm:"type:varchar(50)" json:"owner_contact"`
	Plate        string    `gorm:"type:varchar(20)" json:"plate"`
	RFIDID       string    `gorm:"column:rfid_id;type:varchar(50)" json:"rfid_id"`
	VehicleType  string    `gorm:"type:varchar(20);default:'car'" json:"vehicle_type"`
	CreatedAt    time.Time `gorm:"autoCreateTime" json:"created_at"`

	EntranceExits []EntranceExit `gorm:"foreignKey:VehicleID" json:"entrance_exits,omitempty"`
}
