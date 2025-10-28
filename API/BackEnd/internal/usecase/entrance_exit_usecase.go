package usecase

import "github.com/niflheimdevs/smartparking/internal/domain"

type EntranceExitRepository interface {
	GetAll() ([]domain.EntranceExit, error)
	Info(id uint) (domain.EntranceExit, error)
	VehicleLog(id uint) ([]domain.EntranceExit, error)
}

type EntranceExitUseCase struct {
	repo EntranceExitRepository
}

func NewEntranceExitUseCase(repo EntranceExitRepository) *EntranceExitUseCase {
	return &EntranceExitUseCase{
		repo: repo,
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
