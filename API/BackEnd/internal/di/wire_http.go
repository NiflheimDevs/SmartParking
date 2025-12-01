//go:build wireinject
// +build wireinject

package di

import (
	"github.com/google/wire"

	"github.com/niflheimdevs/smartparking/internal/config"
	"github.com/niflheimdevs/smartparking/internal/db"
	"github.com/niflheimdevs/smartparking/internal/delivery/http"
	http_handler "github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"github.com/niflheimdevs/smartparking/internal/middleware"
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
		repository.NewParkingSpotRepository,
		repository.NewUserRepository,

		// Bind interfaces
		wire.Bind(new(usecase.VehicleRepository), new(*repository.VehicleRepository)),
		wire.Bind(new(usecase.EntranceExitRepository), new(*repository.EntranceExitRepository)),
		wire.Bind(new(usecase.ParkingSpotRepository), new(*repository.ParkingSpotRepository)),
		wire.Bind(new(usecase.UserRepository), new(*repository.UserRepository)),

		// Usecases
		usecase.NewVehicleUseCase,
		usecase.NewEntranceExitUseCase,
		usecase.NewParkingSpotUseCase,
		usecase.NewUserUseCase,
		usecase.NewJWT,

		// Handlers
		http_handler.NewVehicleHandler,
		http_handler.NewEntranceExitHandler,
		http_handler.NewParkingSpotHandler,
		http_handler.NewUserHandler,

		middleware.NewJWTMiddleware,

		// HTTP App
		http.NewHandlers,
		http.NewMiddlewares,
		http.NewHttpApp,
	)
	return &http.App{}, nil
}
