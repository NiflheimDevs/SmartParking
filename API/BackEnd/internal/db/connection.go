package db

import (
	"context"
	"log"

	"github.com/go-redis/redis/v8"
	"github.com/niflheimdevs/smartparking/internal/config"
	"gorm.io/driver/postgres"
	"gorm.io/gorm"
)

func PGConnect(cfg *config.Config) *gorm.DB {
	db, err := gorm.Open(postgres.Open(cfg.Database.URL), &gorm.Config{})
	if err != nil {
		log.Fatalf("Failed to connect database: %v", err)
	}
	return db
}

func RConnect(cfg *config.Config) *redis.Client {
	ctx := context.Background()
	client := redis.NewClient(&redis.Options{
		Addr:     cfg.Redis.Addr,
		Password: cfg.Redis.Password,
		DB:       cfg.Redis.DB,
	})
	_, err := client.Ping(ctx).Result()
	if err != nil {
		log.Fatalf("Failed to connect to Redis: %v", err)
	}
	return client
}
