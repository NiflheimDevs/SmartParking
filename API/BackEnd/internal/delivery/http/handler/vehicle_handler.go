package http_handler

import (
	"net/http"
	"strconv"

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

func (h *VehicleHandler) Update(c *gin.Context) {
	idStr := c.Param("id")
	id, _ := strconv.Atoi(idStr)
	var v domain.Vehicle

	if err := c.ShouldBindJSON(&v); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	if err := h.uc.UpdateVehicle(uint(id), &v); err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, gin.H{
		"message": "vehicle updated successfully",
	})
}

func (h *VehicleHandler) List(c *gin.Context) {
	vehicles, err := h.uc.List()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, vehicles)
}

func (h *VehicleHandler) Info(c *gin.Context) {
	idStr := c.Param("id")
	id, _ := strconv.Atoi(idStr)

	vehicle, err := h.uc.VehicleInfo(uint(id))
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, vehicle)
}

func (h *VehicleHandler) Delete(c *gin.Context) {
	idStr := c.Param("id")
	id, _ := strconv.Atoi(idStr)

	if err := h.uc.DeleteVehicle(uint(id)); err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, gin.H{
		"message": "vehicle deleted successfully",
	})
}
