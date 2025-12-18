package usecase

import (
	"log"
	"time"

	"github.com/niflheimdevs/smartparking/internal/domain"
)

type BanRepository interface {
	IsIPBanned(ip string) (*domain.BannedIP, error)
	BanIP(bannedIP *domain.BannedIP) error
	UnbanIP(ip string) error
}

type BanUseCase struct {
	repo BanRepository
}

func NewBanUseCase(repo BanRepository) *BanUseCase {
	return &BanUseCase{repo: repo}
}

func (uc *BanUseCase) IsIPBanned(ip string) (bool, error) {
	result, err := uc.repo.IsIPBanned(ip)
	if err != nil {
		return false, err
	}

	if result == nil {
		return false, nil
	}

	if result.BannedUntil == nil {
		return true, nil
	} else if result.BannedUntil.Before(time.Now()) {
		err := uc.repo.UnbanIP(ip)
		if err != nil {
			log.Println(err.Error())
		}
		return false, nil
	} else {
		return true, nil
	}
}

func (uc *BanUseCase) BanIP(ip string, duration time.Duration) error {
	bannedUntil := time.Now().Add(duration)
	bannedIP := &domain.BannedIP{
		IPAddress:   ip,
		BannedUntil: &bannedUntil,
	}
	return uc.repo.BanIP(bannedIP)
}
