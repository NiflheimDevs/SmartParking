// internal/di/wire_mqtt.go
//go:build wireinject
// +build wireinject

package di

import (
	"github.com/google/wire"
	"github.com/niflheimdevs/smartparking/internal/config"
	"github.com/niflheimdevs/smartparking/internal/delivery/mqtt"
	mqtt_handler "github.com/niflheimdevs/smartparking/internal/delivery/mqtt/handler"
	"github.com/niflheimdevs/smartparking/internal/repository"
	"github.com/niflheimdevs/smartparking/internal/usecase"
	"gorm.io/gorm"
)

func InitializeMQTTApp(cfg *config.Config, db *gorm.DB) (*mqtt.MQTTClient, error) {
	wire.Build(
		repository.NewEntranceExitRepository,
		repository.NewParkingSpotRepository,
		wire.Bind(new(usecase.EntranceExitRepository), new(*repository.EntranceExitRepository)),
		wire.Bind(new(usecase.ParkingSpotRepository), new(*repository.ParkingSpotRepository)),

		usecase.NewEntranceExitUseCase,
		usecase.NewParkingSpotUseCase,

		mqtt_handler.NewSensorHandler,

		mqtt.InitMQTT,
	)
	return &mqtt.MQTTClient{}, nil
}
