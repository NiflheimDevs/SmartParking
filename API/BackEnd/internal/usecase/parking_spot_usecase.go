package usecase

import (
	"errors"

	"github.com/niflheimdevs/smartparking/internal/domain"
	"github.com/niflheimdevs/smartparking/internal/dto"
)

type ParkingSpotRepository interface {
	GetAll() ([]domain.ParkingSpot, error)
	FindFree() (*domain.ParkingSpot, error)
	Get(number string) (*domain.ParkingSpot, error)
	Update(number string, is_occupied bool) error
}

type ParkingSpotUseCase struct {
	EntranceExitUseCase *EntranceExitUseCase
	repo                ParkingSpotRepository
}

func NewParkingSpotUseCase(
	eeuc *EntranceExitUseCase,
	repo ParkingSpotRepository,
) *ParkingSpotUseCase {
	return &ParkingSpotUseCase{
		EntranceExitUseCase: eeuc,
		repo:                repo,
	}
}

func (uc *ParkingSpotUseCase) List() ([]dto.ParkingSpotDTO, error) {
	var pssdto []dto.ParkingSpotDTO
	pss, err := uc.repo.GetAll()
	if err != nil {
		return nil, err
	}
	for _, ps := range pss[1:] {
		var psdto dto.ParkingSpotDTO
		psdto.Number = ps.SpotNumber
		psdto.IsOccupied = ps.IsOccupied
		if ps.IsOccupied {
			vehicle, ee, err := uc.EntranceExitUseCase.FindParkedVehicle(ps.ID)
			if err != nil {
				return nil, err
			}
			psdto.Vehicle = vehicle
			psdto.EntranceTime = &ee.EntranceTime
		} else {
			psdto.Vehicle = nil
			psdto.EntranceTime = nil
		}
		pssdto = append(pssdto, psdto)
	}
	return pssdto, nil
}

func (uc *ParkingSpotUseCase) FindFree() (*domain.ParkingSpot, error) {
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
	err = uc.repo.Update(number, is_occupied)
	return space.ID, err
}
