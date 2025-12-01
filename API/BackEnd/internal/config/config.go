package config

import (
	"os"
)

type Config struct {
	Server struct {
		Port string
	}
	Database struct {
		URL string
	}
	MQTT struct {
		Broker   string
		ClientID string
	}
	Constant struct {
		PricePerHour int64
		JWTKeysPath  string
	}
}

func Load() *Config {
	cfg := &Config{}

	cfg.Server.Port = getEnv("SERVER_PORT", ":8080")
	cfg.Database.URL = getEnv("DATABASE_URL", "postgres://postgres:postgres@postgres:5432/parking?sslmode=disable")
	cfg.MQTT.Broker = getEnv("MQTT_BROKER", "tcp://mosquitto:1883")
	cfg.MQTT.ClientID = getEnv("MQTT_CLIENT_ID", "smart-parking-server")

	cfg.Constant.PricePerHour = 500000
	cfg.Constant.JWTKeysPath = "./internal/jwt"
	return cfg
}

func getEnv(key, fallback string) string {
	if value, exists := os.LookupEnv(key); exists {
		return value
	}
	return fallback
}
