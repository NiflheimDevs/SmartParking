package usecase

import (
	"errors"
	"log"

	"github.com/niflheimdevs/smartparking/internal/domain"
)

type ParkingSpotRepository interface {
	GetAll() ([]domain.ParkingSpot, error)
	FindFree() (domain.ParkingSpot, error)
	Get(number string) (domain.ParkingSpot, error)
	Update(number string, is_occupied bool) error
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
	pss, err := uc.repo.GetAll()
	return pss[1:], err
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
	log.Print(space.ID, " ", space.SpotNumber)
	err = uc.repo.Update(number, is_occupied)
	return space.ID, err
}
