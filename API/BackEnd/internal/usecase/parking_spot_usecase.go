package usecase

import "github.com/niflheimdevs/smartparking/internal/domain"

type ParkingSpotRepository interface {
	GetAll() ([]domain.ParkingSpot, error)
}

type ParkingSpotUseCase struct {
	repo ParkingSpotRepository
}

func NewParkingSpotUseCase(repo ParkingSpotRepository) *ParkingSpotUseCase {
	return &ParkingSpotUseCase{
		repo: repo,
	}
}

func (uc *ParkingSpotUseCase) List() ([]domain.ParkingSpot, error) {
	return uc.repo.GetAll()
}
