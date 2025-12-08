//go:build wireinject
// +build wireinject

package di

import (
	"github.com/google/wire"

	"github.com/niflheimdevs/smartparking/internal/config"
	"github.com/niflheimdevs/smartparking/internal/db"
	"github.com/niflheimdevs/smartparking/internal/delivery/http"
	http_handler "github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"github.com/niflheimdevs/smartparking/internal/delivery/mqtt"
	mqtt_handler "github.com/niflheimdevs/smartparking/internal/delivery/mqtt/handler"
	"github.com/niflheimdevs/smartparking/internal/middleware"
	"github.com/niflheimdevs/smartparking/internal/repository"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

func InitializeHttpApp() (*http.App, error) {
	wire.Build(
		config.Load,
		db.Connect,

		CoreSet, // <--- HERE

		repository.NewUserRepository,
		wire.Bind(new(usecase.UserRepository), new(*repository.UserRepository)),
		usecase.NewUserUseCase,
		usecase.NewJWT,

		mqtt_handler.NewSensorHandler,
		mqtt.InitMQTTClient,

		http_handler.NewGateHandler,
		http_handler.NewVehicleHandler,
		http_handler.NewEntranceExitHandler,
		http_handler.NewParkingSpotHandler,
		http_handler.NewUserHandler,

		middleware.NewJWTMiddleware,

		http.NewHandlers,
		http.NewMiddlewares,
		http.NewHttpApp,
	)
	return &http.App{}, nil
}
