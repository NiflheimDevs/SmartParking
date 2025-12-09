//go:build wireinject
// +build wireinject

package di

import (
	"github.com/google/wire"
	"gorm.io/gorm"

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

type App struct {
	HttpApp    *http.HTTPApp
	MQTTClient *mqtt.MQTTClient
	Config     *config.Config
	DB         *gorm.DB
}

func InitializeApp() (*App, error) {
	wire.Build(
		config.Load,
		db.Connect,

		// Repositories
		repository.NewVehicleRepository,
		repository.NewEntranceExitRepository,
		repository.NewParkingSpotRepository,
		repository.NewUserRepository,

		// Bind
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

		mqtt_handler.NewSensorHandler,
		mqtt.InitMQTTClient,

		http_handler.NewGateHandler,
		http_handler.NewVehicleHandler,
		http_handler.NewEntranceExitHandler,
		http_handler.NewParkingSpotHandler,
		http_handler.NewUserHandler,

		middleware.NewJWTMiddleware,

		wire.Struct(new(http.Handlers), "*"),
		wire.Struct(new(http.Middlewares), "*"),

		mqtt.InitMQTT,

		http.NewHttpApp,

		wire.Struct(new(App), "*"),
	)
	return &App{}, nil
}
