package domain

import "errors"

var (
	ErrIPBanned            = errors.New("ip is banned")
	ErrIPBannedByRateLimit = errors.New("ip banned due to excessive requests")
)
