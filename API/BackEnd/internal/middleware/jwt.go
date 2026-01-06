package middleware

import (
	"net/http"
	"strings"

	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type JWTMiddleware struct {
	uc *usecase.JWT
}

func NewJWTMiddleware(uc *usecase.JWT) *JWTMiddleware {
	return &JWTMiddleware{uc}
}

func (m *JWTMiddleware) Validate() gin.HandlerFunc {
	return func(ctx *gin.Context) {
		authHeader := ctx.GetHeader("Authorization")
		if authHeader == "" {
			ctx.JSON(http.StatusUnauthorized, gin.H{"error": "authorization header required"})
			ctx.Abort()
			return
		}

		parts := strings.Split(authHeader, " ")
		if len(parts) != 2 || strings.ToLower(parts[0]) != "bearer" {
			ctx.JSON(http.StatusUnauthorized, gin.H{"error": "invalid authorization header format"})
			ctx.Abort()
			return
		}

		tokenStr := parts[1]

		claims, err := m.uc.VerifyToken(tokenStr)
		if err != nil {
			ctx.JSON(http.StatusUnauthorized, gin.H{"error": err.Error()})
			ctx.Abort()
			return
		}

		if userID, ok := claims["sub"].(float64); ok {
			ctx.Set("user_id", uint(userID))
		}

		ctx.Next()
	}
}
