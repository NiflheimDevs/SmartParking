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
	return nil, nil
}
