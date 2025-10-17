//go:build wireinject
// +build wireinject

package di

import (
	"github.com/google/wire"
	"github.com/niflheimdevs/smartparking/internal/config"
	"github.com/niflheimdevs/smartparking/internal/db"
	"github.com/niflheimdevs/smartparking/internal/delivery/http"
	"github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"github.com/niflheimdevs/smartparking/internal/repository"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

func InitializeApp() (*http.App, error) {
	wire.Build(
		// Core
		config.Load,
		db.Connect,

		// Repositories
		repository.NewVehicleRepository,

		// Bind interface to implementation
		wire.Bind(new(usecase.VehicleRepository), new(*repository.VehicleRepository)),

		// Usecases
		usecase.NewVehicleUseCase,

		// Handlers
		handler.NewVehicleHandler,

		// Router (HTTP app)
		http.NewApp,
	)
	return &http.App{}, nil
}
