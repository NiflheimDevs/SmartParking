// internal/di/wire_mqtt.go
//go:build wireinject
// +build wireinject

package di

import (
	mqtt_a "github.com/eclipse/paho.mqtt.golang"
	"github.com/google/wire"
	"github.com/niflheimdevs/smartparking/internal/config"
	"github.com/niflheimdevs/smartparking/internal/delivery/mqtt"
	mqtt_handler "github.com/niflheimdevs/smartparking/internal/delivery/mqtt/handler"
	"gorm.io/gorm"
)

func InitializeMQTTApp(cfg *config.Config, db *gorm.DB, client mqtt_a.Client) (*mqtt.MQTTClient, error) {
	wire.Build(
		CoreSet, // <--- HERE

		mqtt_handler.NewSensorHandler,

		mqtt.InitMQTT,
	)
	return &mqtt.MQTTClient{}, nil
}
