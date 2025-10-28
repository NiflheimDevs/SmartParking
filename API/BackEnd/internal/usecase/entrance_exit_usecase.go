package usecase

import (
	"time"

	"github.com/niflheimdevs/smartparking/internal/domain"
)

type EntranceExitRepository interface {
	GetAll() ([]domain.EntranceExit, error)
	Info(id uint) (domain.EntranceExit, error)
	VehicleLog(id uint) ([]domain.EntranceExit, error)
	Enter(ee domain.EntranceExit) error
}

type EntranceExitUseCase struct {
	repo           EntranceExitRepository
	VehicleUseCase *VehicleUseCase
}

func NewEntranceExitUseCase(
	repo EntranceExitRepository,
	vuc *VehicleUseCase,
) *EntranceExitUseCase {
	return &EntranceExitUseCase{
		repo:           repo,
		VehicleUseCase: vuc,
	}
}

func (uc *EntranceExitUseCase) List() ([]domain.EntranceExit, error) {
	return uc.repo.GetAll()
}

func (uc *EntranceExitUseCase) Info(id uint) (domain.EntranceExit, error) {
	return uc.repo.Info(id)
}

func (uc *EntranceExitUseCase) VehicleInfo(id uint) ([]domain.EntranceExit, error) {
	return uc.repo.VehicleLog(id)
}

func (uc *EntranceExitUseCase) Enter(id string) error {
	vehicle, err := uc.VehicleUseCase.VehicleInfoRFID(id)
	if err != nil {
		return err
	}
	ee := domain.EntranceExit{
		VehicleID:    vehicle.ID,
		EntranceTime: time.Now(),
	}
	return uc.repo.Enter(ee)
}
