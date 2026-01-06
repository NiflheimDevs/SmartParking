//go:build wireinject
// +build wireinject

package di

import (
	"github.com/go-redis/redis/v8"
	"github.com/google/wire"
	"gorm.io/gorm"

	"github.com/niflheimdevs/smartparking/internal/config"
	"github.com/niflheimdevs/smartparking/internal/db"
	"github.com/niflheimdevs/smartparking/internal/delivery/http"
	http_handler "github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"github.com/niflheimdevs/smartparking/internal/delivery/mqtt"
	mqtt_handler "github.com/niflheimdevs/smartparking/internal/delivery/mqtt/handler"
	"github.com/niflheimdevs/smartparking/internal/middleware"
	pg_repository "github.com/niflheimdevs/smartparking/internal/repository/postgres"
	r_repository "github.com/niflheimdevs/smartparking/internal/repository/redis"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type App struct {
	HttpApp    *http.HTTPApp
	MQTTClient *mqtt.MQTTClient
	Config     *config.Config
	PGDB       *gorm.DB
	RDB        *redis.Client
}

func InitializeApp() (*App, error) {
	wire.Build(
		config.Load,
		db.PGConnect,
		db.RConnect,

		// Repositories
		pg_repository.NewVehicleRepository,
		pg_repository.NewEntranceExitRepository,
		pg_repository.NewParkingSpotRepository,
		pg_repository.NewUserRepository,
		pg_repository.NewBanRepository,

		r_repository.NewLoggerRepository,

		// Bind
		wire.Bind(new(usecase.VehicleRepository), new(*pg_repository.VehicleRepository)),
		wire.Bind(new(usecase.EntranceExitRepository), new(*pg_repository.EntranceExitRepository)),
		wire.Bind(new(usecase.ParkingSpotRepository), new(*pg_repository.ParkingSpotRepository)),
		wire.Bind(new(usecase.UserRepository), new(*pg_repository.UserRepository)),
		wire.Bind(new(usecase.BanRepository), new(*pg_repository.BanRepository)),

		wire.Bind(new(usecase.LoggerRepository), new(*r_repository.LoggerRepository)),

		// Usecases
		usecase.NewVehicleUseCase,
		usecase.NewEntranceExitUseCase,
		usecase.NewParkingSpotUseCase,
		usecase.NewUserUseCase,
		usecase.NewJWT,
		usecase.NewBanUseCase,
		usecase.NewIPLoggerUseCase,

		mqtt_handler.NewSensorHandler,
		mqtt.InitMQTTClient,

		http_handler.NewGateHandler,
		http_handler.NewVehicleHandler,
		http_handler.NewEntranceExitHandler,
		http_handler.NewParkingSpotHandler,
		http_handler.NewUserHandler,

		middleware.NewJWTMiddleware,
		middleware.NewIPBanMiddleware,
		middleware.NewIPLoggerMiddleware,

		wire.Struct(new(http.Handlers), "*"),
		wire.Struct(new(http.Middlewares), "*"),

		mqtt.InitMQTT,

		http.NewHttpApp,

		wire.Struct(new(App), "*"),
	)
	return &App{}, nil
}
