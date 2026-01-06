package middleware

import (
	"strings"

	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type IPBanMiddleware struct {
	uc *usecase.BanUseCase
}

func NewIPBanMiddleware(uc *usecase.BanUseCase) *IPBanMiddleware {
	return &IPBanMiddleware{uc: uc}
}

func GetClientIP(c *gin.Context) string {
	if ip := c.GetHeader("CF-Connecting-IP"); ip != "" {
		return ip
	}
	if ip := c.GetHeader("X-Forwarded-For"); ip != "" {
		return strings.Split(ip, ",")[0]
	}
	return c.ClientIP()
}

func (m *IPBanMiddleware) CheckIPBan() gin.HandlerFunc {
	return func(c *gin.Context) {
		ip := GetClientIP(c)
		banned, err := m.uc.IsIPBanned(ip)
		if err != nil {
			c.JSON(500, gin.H{"error": "Internal server error"})
			c.Abort()
			return
		}
		if banned {
			c.JSON(403, gin.H{"error": "Your IP is banned"})
			c.Abort()
			return
		}
		c.Next()
	}
}
