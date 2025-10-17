package repository

import (
	"github.com/niflheimdevs/smartparking/internal/domain"
	"gorm.io/gorm"
)

type VehicleRepository struct {
	db *gorm.DB
}

func NewVehicleRepository(db *gorm.DB) *VehicleRepository {
	return &VehicleRepository{
		db: db,
	}
}

func (r *VehicleRepository) Create(v *domain.Vehicle) error {
	return r.db.Create(v).Error
}

func (r *VehicleRepository) GetAll() ([]domain.Vehicle, error) {
	var vehicles []domain.Vehicle
	err := r.db.Find(&vehicles).Error
	return vehicles, err
}
