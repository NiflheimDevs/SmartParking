package usecase

import (
	"time"

	"github.com/niflheimdevs/smartparking/internal/domain"
)

type LoggerRepository interface {
	IncrementIPRequestCount(ip string) error
	GetIPRequestCount(ip string) (int64, error)
}

type IPLoggerUseCase struct {
	repo       LoggerRepository
	BanUseCase *BanUseCase
}

func NewIPLoggerUseCase(repo LoggerRepository, buc *BanUseCase) *IPLoggerUseCase {
	return &IPLoggerUseCase{
		repo:       repo,
		BanUseCase: buc,
	}
}

func (uc *IPLoggerUseCase) HandleIPRequest(ip string) error {
	key := "ip_request:" + ip

	if is, _ := uc.BanUseCase.IsIPBanned(ip); is {
		return domain.ErrIPBanned
	}

	err := uc.repo.IncrementIPRequestCount(key)
	if err != nil {
		return err
	}

	count, err := uc.repo.GetIPRequestCount(key)
	if err != nil {
		return err
	}

	if count > 5 {
		if err := uc.BanUseCase.BanIP(ip, 24*time.Hour); err != nil {
			return err
		}
		return domain.ErrIPBannedByRateLimit
	}

	return nil
}
