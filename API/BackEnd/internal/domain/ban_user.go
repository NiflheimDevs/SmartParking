package domain

import "time"

type BannedIP struct {
	ID          uint   `gorm:"primaryKey"`
	IPAddress   string `gorm:"uniqueIndex;size:45"` // IPv4 + IPv6
	Reason      string
	BannedUntil *time.Time // nil = permanent
	CreatedAt   time.Time
}
