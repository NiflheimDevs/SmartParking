package dto

import (
	"time"

	"github.com/niflheimdevs/smartparking/internal/domain"
)

type ParkingSpotDTO struct {
	Number       string          `json:"number"`
	IsOccupied   bool            `json:"is_occupied"`
	Vehicle      *domain.Vehicle `json:"vehicle"`
	EntranceTime *time.Time      `json:"entrance_time"`
}
