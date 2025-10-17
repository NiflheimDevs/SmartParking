package mqtt_handler

import (
	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type SensorHandler struct {
	EntranceExitUseCase *usecase.EntranceExitUseCase
	ParkingSpotUseCAse  *usecase.ParkingSpotUseCase
}

func NewSensorHandler(eeuc *usecase.EntranceExitUseCase, psuc *usecase.ParkingSpotUseCase) *SensorHandler {
	return &SensorHandler{
		EntranceExitUseCase: eeuc,
		ParkingSpotUseCAse:  psuc,
	}
}

// Payload structure (sent from IoT device)
type EntrancePayload struct {
	RFID      string `json:"rfid"`
	Timestamp int64  `json:"timestamp"`
}

func (h *SensorHandler) OnEntrance(client mqtt.Client, msg mqtt.Message) {
}

func (h *SensorHandler) OnExit(client mqtt.Client, msg mqtt.Message) {
}
