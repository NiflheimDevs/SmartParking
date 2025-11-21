package usecase

import (
	"time"

	"github.com/niflheimdevs/smartparking/internal/config"
	"github.com/niflheimdevs/smartparking/internal/domain"
)

type EntranceExitRepository interface {
	GetAll() ([]domain.EntranceExit, error)
	Info(id uint) (domain.EntranceExit, error)
	VehicleLog(id uint) ([]domain.EntranceExit, error)
	Enter(ee domain.EntranceExit) error
	FindVehicleEnter(id uint) (domain.EntranceExit, error)
	Exit(ee domain.EntranceExit) error
}

type EntranceExitUseCase struct {
	cfg            *config.Config
	repo           EntranceExitRepository
	VehicleUseCase *VehicleUseCase
}

func NewEntranceExitUseCase(
	cfg *config.Config,
	repo EntranceExitRepository,
	vuc *VehicleUseCase,
) *EntranceExitUseCase {
	return &EntranceExitUseCase{
		cfg:            cfg,
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

func (uc *EntranceExitUseCase) Exit(id string) (int64, error) {
	vehicle, err := uc.VehicleUseCase.VehicleInfoRFID(id)
	if err != nil {
		return 0, err
	}

	log, err := uc.repo.FindVehicleEnter(vehicle.ID)
	if err != nil {
		return 0, err
	}

	log.ExitTime = time.Now()

	price := log.ExitTime.Sub(log.EntranceTime).Hours() * float64(uc.cfg.Constant.PricePerHour)

	err = uc.repo.Exit(log)

	return int64(price), err
}
