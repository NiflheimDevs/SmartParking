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

func (r *VehicleRepository) Update(id uint, v *domain.Vehicle) error {
	var existing domain.Vehicle
	err := r.db.Model(&existing).
		Where("id = ?", id).
		Select("owner_name", "owner_contact", "plate", "rfid_id", "vehicle_type").
		Updates(v).Error
	if err != nil {
		return err
	}
	return nil
}
