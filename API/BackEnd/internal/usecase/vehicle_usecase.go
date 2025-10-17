package usecase

import "github.com/niflheimdevs/smartparking/internal/domain"

type VehicleRepository interface {
	Create(v *domain.Vehicle) error
	GetAll() ([]domain.Vehicle, error)
}

type VehicleUseCase struct {
	repo VehicleRepository
}

func NewVehicleUseCase(repo VehicleRepository) *VehicleUseCase {
	return &VehicleUseCase{
		repo: repo,
	}
}

func (uc *VehicleUseCase) RegisterVehicle(v *domain.Vehicle) error {
	return uc.repo.Create(v)
}

func (uc *VehicleUseCase) List() ([]domain.Vehicle, error) {
	return uc.repo.GetAll()
}
