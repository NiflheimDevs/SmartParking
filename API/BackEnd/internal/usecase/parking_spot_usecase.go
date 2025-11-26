package usecase

import (
	"errors"

	"github.com/niflheimdevs/smartparking/internal/domain"
)

type ParkingSpotRepository interface {
	GetAll() ([]domain.ParkingSpot, error)
	FindFree() (domain.ParkingSpot, error)
	Get(number string) (domain.ParkingSpot, error)
	Update(number string, is_occupied bool) (uint, error)
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

func (uc *ParkingSpotUseCase) FindFree() (domain.ParkingSpot, error) {
	return uc.repo.FindFree()
}

func (uc *ParkingSpotUseCase) Update(number string, is_occupied bool) (uint, error) {
	space, err := uc.repo.Get(number)
	if err != nil {
		return 0, err
	}
	if space.IsOccupied == is_occupied {
		return 0, errors.New("Status not changed")
	}
	spotID, err := uc.repo.Update(number, is_occupied)
	return spotID, err
}
