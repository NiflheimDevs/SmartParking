package di

import (
	"github.com/google/wire"
	"github.com/niflheimdevs/smartparking/internal/repository"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

var CoreSet = wire.NewSet(
	// Repositories
	repository.NewVehicleRepository,
	repository.NewEntranceExitRepository,
	repository.NewParkingSpotRepository,

	// Bind
	wire.Bind(new(usecase.VehicleRepository), new(*repository.VehicleRepository)),
	wire.Bind(new(usecase.EntranceExitRepository), new(*repository.EntranceExitRepository)),
	wire.Bind(new(usecase.ParkingSpotRepository), new(*repository.ParkingSpotRepository)),

	// Usecases
	usecase.NewVehicleUseCase,
	usecase.NewEntranceExitUseCase,
	usecase.NewParkingSpotUseCase,
)
