package http_handler

import (
	"net/http"

	"github.com/gin-gonic/gin"
	mqtt_handler "github.com/niflheimdevs/smartparking/internal/delivery/mqtt/handler"
)

type GateHandler struct {
	MQTTHandler *mqtt_handler.SensorHandler
}

func NewGateHandler(mqttHandler *mqtt_handler.SensorHandler) *GateHandler {
	return &GateHandler{
		MQTTHandler: mqttHandler,
	}
}

func (h *GateHandler) Control(c *gin.Context) {
	type gateRequest struct {
		Gate  string `json:"gate"`
		State bool   `json:"state"`
	}

	var gr gateRequest
	if err := c.ShouldBindJSON(&gr); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	h.MQTTHandler.Gate(gr.Gate, gr.State)
	c.Status(http.StatusOK)
}
