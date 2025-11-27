package http_handler

import (
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type ParkingSpotHandler struct {
	uc *usecase.ParkingSpotUseCase
}

func NewParkingSpotHandler(uc *usecase.ParkingSpotUseCase) *ParkingSpotHandler {
	return &ParkingSpotHandler{
		uc: uc,
	}
}

func (h *ParkingSpotHandler) GetStatus(c *gin.Context) {
	pss, err := h.uc.List()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, pss)
}
