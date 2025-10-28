package http_handler

import (
	"net/http"
	"strconv"

	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type EntranceExitHandler struct {
	uc *usecase.EntranceExitUseCase
}

func NewEntranceExitHandler(uc *usecase.EntranceExitUseCase) *EntranceExitHandler {
	return &EntranceExitHandler{uc}
}

func (h *EntranceExitHandler) List(c *gin.Context) {
	ees, err := h.uc.List()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, ees)
}

func (h *EntranceExitHandler) Info(c *gin.Context) {
	idStr := c.Param("id")
	id, _ := strconv.Atoi(idStr)

	vehicle, err := h.uc.Info(uint(id))
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, vehicle)
}

func (h *EntranceExitHandler) VehicleLog(c *gin.Context) {
	idStr := c.Param("id")
	id, _ := strconv.Atoi(idStr)

	log, err := h.uc.VehicleInfo(uint(id))
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, log)
}
