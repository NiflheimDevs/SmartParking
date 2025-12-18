package redis

import (
	"time"

	"github.com/go-redis/redis/v8"
)

type LoggerRepository struct {
	Client *redis.Client
}

func NewLoggerRepository(client *redis.Client) *LoggerRepository {
	return &LoggerRepository{Client: client}
}

func (r *LoggerRepository) IncrementIPRequestCount(ip string) error {
	ctx := r.Client.Context()

	// Increment the request count
	val, err := r.Client.Incr(ctx, ip).Result()
	if err != nil {
		return err
	}

	// Set TTL only if key is new
	if val == 1 {
		r.Client.Expire(ctx, ip, 24*time.Hour)
	}

	return nil
}

func (r *LoggerRepository) GetIPRequestCount(ip string) (int64, error) {
	ctx := r.Client.Context()

	val, err := r.Client.Get(ctx, ip).Int64()
	if err != nil {
		return 0, err
	}

	return val, nil
}
