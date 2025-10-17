package repository

import (
	"github.com/niflheimdevs/smartparking/internal/domain"
	"gorm.io/gorm"
)

type ParkingSpotRepository struct {
	db *gorm.DB
}

func NewParkingSpotRepository(db *gorm.DB) *ParkingSpotRepository {
	return &ParkingSpotRepository{
		db: db,
	}
}

func (r *ParkingSpotRepository) GetAll() ([]domain.ParkingSpot, error) {
	return nil, nil
}
