package usecase

import "github.com/niflheimdevs/smartparking/internal/domain"

type EntranceExitRepository interface {
	GetAll() ([]domain.EntranceExit, error)
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
