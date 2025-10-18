//go:build wireinject
// +build wireinject

package di

import (
	"github.com/google/wire"

	"github.com/niflheimdevs/smartparking/internal/config"
	"github.com/niflheimdevs/smartparking/internal/db"
	"github.com/niflheimdevs/smartparking/internal/delivery/http"
	http_handler "github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"github.com/niflheimdevs/smartparking/internal/repository"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

func InitializeHttpApp() (*http.App, error) {
	wire.Build(
		// Core
		config.Load,
		db.Connect,

		// Repositories
		repository.NewVehicleRepository,
		repository.NewEntranceExitRepository,

		// Bind interfaces
		wire.Bind(new(usecase.VehicleRepository), new(*repository.VehicleRepository)),
		wire.Bind(new(usecase.EntranceExitRepository), new(*repository.EntranceExitRepository)),

		// Usecases
		usecase.NewVehicleUseCase,
		usecase.NewEntranceExitUseCase,

		// Handlers
		http_handler.NewVehicleHandler,
		http_handler.NewEntranceExitHandler,

		// HTTP App
		http.NewHttpApp,
	)
	return &http.App{}, nil
}
