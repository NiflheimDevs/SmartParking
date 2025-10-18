package usecase

import (
	"errors"

	"github.com/niflheimdevs/smartparking/internal/domain"
)

type VehicleRepository interface {
	Create(v *domain.Vehicle) error
	GetAll() ([]domain.Vehicle, error)
	Update(id uint, v *domain.Vehicle) error
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
	if v.OwnerName == "" {
		return errors.New("owner name cannot be empty")
	}
	return uc.repo.Create(v)
}

func (uc *VehicleUseCase) List() ([]domain.Vehicle, error) {
	return uc.repo.GetAll()
}

func (uc *VehicleUseCase) UpdateVehicle(id uint, v *domain.Vehicle) error {
	if v.OwnerName == "" {
		return errors.New("owner name cannot be empty")
	}
	return uc.repo.Update(id, v)
}
