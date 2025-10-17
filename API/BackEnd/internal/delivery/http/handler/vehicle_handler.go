package handler

import (
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/domain"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type VehicleHandler struct {
	uc *usecase.VehicleUseCase
}

func NewVehicleHandler(uc *usecase.VehicleUseCase) *VehicleHandler {
	return &VehicleHandler{uc}
}

func (h *VehicleHandler) Register(c *gin.Context) {
	var v domain.Vehicle
	if err := c.ShouldBindJSON(&v); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	if err := h.uc.RegisterVehicle(&v); err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusCreated, v)
}

func (h *VehicleHandler) List(c *gin.Context) {
	vehicles, err := h.uc.List()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, vehicles)
}
