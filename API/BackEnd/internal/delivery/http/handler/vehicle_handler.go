package http_handler

import (
	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type VehicleHandler struct {
	uc *usecase.VehicleUseCase
}

func NewVehicleHandler(uc *usecase.VehicleUseCase) *VehicleHandler {
	return &VehicleHandler{uc}
}

func (h *VehicleHandler) Register(c *gin.Context) {
}

func (h *VehicleHandler) List(c *gin.Context) {
}
