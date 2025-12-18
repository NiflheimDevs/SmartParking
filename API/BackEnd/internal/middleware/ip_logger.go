package middleware

import (
	"errors"

	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/domain"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type IPLoggerMiddleware struct {
	uc *usecase.IPLoggerUseCase
}

func NewIPLoggerMiddleware(uc *usecase.IPLoggerUseCase) *IPLoggerMiddleware {
	return &IPLoggerMiddleware{uc: uc}
}

func (m *IPLoggerMiddleware) IPLogger() gin.HandlerFunc {
	return func(c *gin.Context) {
		ip := c.ClientIP()

		err := m.uc.HandleIPRequest(ip)
		if err != nil {
			switch {
			case errors.Is(err, domain.ErrIPBanned),
				errors.Is(err, domain.ErrIPBannedByRateLimit):

				c.AbortWithStatusJSON(403, gin.H{
					"error": err.Error(),
				})
				return

			default:
				// unexpected error
				c.AbortWithStatusJSON(500, gin.H{
					"error": "internal server error",
				})
				return
			}
		}

		c.Next()
	}
}
