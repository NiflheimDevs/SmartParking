package repository

import (
	"github.com/niflheimdevs/smartparking/internal/domain"
	"gorm.io/gorm"
)

type EntranceExitRepository struct {
	db *gorm.DB
}

func NewEntranceExitRepository(db *gorm.DB) *EntranceExitRepository {
	return &EntranceExitRepository{
		db: db,
	}
}

func (r *EntranceExitRepository) GetAll() ([]domain.EntranceExit, error) {
	var ees []domain.EntranceExit
	err := r.db.Find(&ees).Error
	return ees, err
}

func (r *EntranceExitRepository) Info(id uint) (domain.EntranceExit, error) {
	var ee domain.EntranceExit
	err := r.db.First(&ee, id).Error
	return ee, err
}

func (r *EntranceExitRepository) VehicleLog(id uint) ([]domain.EntranceExit, error) {
	var log []domain.EntranceExit
	err := r.db.Where("vehicle_id = ?", id).Find(&log).Error
	return log, err
}

func (r *EntranceExitRepository) Enter(ee domain.EntranceExit) error {
	return r.db.Create(ee).Error
}
