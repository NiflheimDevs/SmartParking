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
	var spots []domain.ParkingSpot
	err := r.db.Find(&spots).Error
	return spots, err
}

func (r *ParkingSpotRepository) FindFree() (domain.ParkingSpot, error) {
	var spot domain.ParkingSpot
	err := r.db.First(&spot).Where("is_occupied = false").Error
	return spot, err
}
