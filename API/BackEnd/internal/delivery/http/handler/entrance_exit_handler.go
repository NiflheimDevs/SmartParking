package http_handler

import (
	"net/http"

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
