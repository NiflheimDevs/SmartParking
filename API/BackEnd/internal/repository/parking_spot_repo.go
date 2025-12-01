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
	err := r.db.Order("id ASC").Find(&spots).Error
	return spots, err
}

func (r *ParkingSpotRepository) FindFree() (domain.ParkingSpot, error) {
	var spot domain.ParkingSpot
	err := r.db.Where("is_occupied = false and spot_number != ?", "In the way").First(&spot).Error
	return spot, err
}

func (r *ParkingSpotRepository) Get(number string) (domain.ParkingSpot, error) {
	var spot domain.ParkingSpot
	err := r.db.Where("spot_number = ?", number).First(&spot).Error
	return spot, err
}

func (r *ParkingSpotRepository) Update(number string, is_occupied bool) error {
	return r.db.Model(&domain.ParkingSpot{}).Where("spot_number = ?", number).UpdateColumn("is_occupied", is_occupied).Error
}
