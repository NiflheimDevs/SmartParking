package repository

import (
	"errors"

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

func (r *VehicleRepository) GetInfo(id uint) (*domain.Vehicle, error) {
	var vehicle domain.Vehicle
	err := r.db.First(&vehicle, id).Error
	return &vehicle, err
}

func (r *VehicleRepository) Delete(id uint) error {
	return r.db.Delete(&domain.Vehicle{}, id).Error
}

func (r *VehicleRepository) CheckRFID(id string) (bool, error) {
	var v domain.Vehicle
	err := r.db.Select("id").Where("rfid_id = ?", id).First(&v).Error
	if err != nil {
		if errors.Is(err, gorm.ErrRecordNotFound) {
			return false, nil
		}
		return false, err
	}
	return true, nil
}

func (r *VehicleRepository) VehicleInfoRFID(id string) (*domain.Vehicle, error) {
	var v domain.Vehicle
	err := r.db.Where("rfid_id = ?", id).First(&v).Error
	return &v, err
}
