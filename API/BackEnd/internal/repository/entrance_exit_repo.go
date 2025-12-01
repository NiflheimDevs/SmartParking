package repository

import (
	"time"

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
	err := r.db.Preload("Vehicle").Find(&ees).Error
	return ees, err
}

func (r *EntranceExitRepository) Info(id uint) (*domain.EntranceExit, error) {
	var ee domain.EntranceExit
	err := r.db.Preload("Vehicle").First(&ee, id).Error
	return &ee, err
}

func (r *EntranceExitRepository) VehicleLog(id uint) ([]domain.EntranceExit, error) {
	var log []domain.EntranceExit
	err := r.db.Preload("Vehicle").Where("vehicle_id = ?", id).Find(&log).Error
	return log, err
}

func (r *EntranceExitRepository) Enter(ee *domain.EntranceExit) error {
	return r.db.Create(ee).Error
}

// check that only one row exists
func (r *EntranceExitRepository) FindVehicleEnter(id uint) (*domain.EntranceExit, error) {
	var ee domain.EntranceExit
	err := r.db.Where("vehicle_id = ? and exit_time = ?", id, time.Time{}).First(&ee).Error
	return &ee, err
}

func (r *EntranceExitRepository) Exit(ee *domain.EntranceExit) error {
	return r.db.Where("id = ?", ee.ID).Updates(ee).Error
}

func (r *EntranceExitRepository) ParkVehicle(spaceID uint) error {
	return r.db.Exec("UPDATE entrance_exits SET spot_id = ? WHERE id = (SELECT id FROM entrance_exits WHERE spot_id = 1 ORDER BY id DESC LIMIT 1)", spaceID).Error
}
