package repository

import (
	"github.com/niflheimdevs/smartparking/internal/domain"
	"gorm.io/gorm"
)

type BanRepository struct {
	DB *gorm.DB
}

func NewBanRepository(db *gorm.DB) *BanRepository {
	return &BanRepository{
		DB: db,
	}
}

func (r *BanRepository) IsIPBanned(ip string) (*domain.BannedIP, error) {
	var result domain.BannedIP
	err := r.DB.Where("ip_address = ?", ip).First(&result).Error
	if err != nil && err == gorm.ErrRecordNotFound {
		return nil, nil
	}
	return &result, err
}

func (r *BanRepository) BanIP(bannedIP *domain.BannedIP) error {
	return r.DB.Create(bannedIP).Error
}

func (r *BanRepository) UnbanIP(ip string) error {
	return r.DB.Where("ip_address = ?", ip).Delete(&domain.BannedIP{}).Error
}
