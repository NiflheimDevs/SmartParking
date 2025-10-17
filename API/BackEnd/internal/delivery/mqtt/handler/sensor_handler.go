package mqtt

import (
	"encoding/json"
	"log"
	"time"

	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

// Handler connects MQTT messages to business logic
type Handler struct {
	EntranceExitUseCase *usecase.EntranceExitUseCase
}

func NewHandler(uc *usecase.EntranceExitUseCase) *Handler {
	return &Handler{EntranceExitUseCase: uc}
}

// Payload structure (sent from IoT device)
type EntrancePayload struct {
	RFID      string `json:"rfid"`
	Timestamp int64  `json:"timestamp"`
}

// Called when a car enters
func (h *Handler) OnEntrance(client mqtt.Client, msg mqtt.Message) {
	log.Printf("🚗 Received entrance event on topic [%s]: %s", msg.Topic(), string(msg.Payload()))

	var data EntrancePayload
	if err := json.Unmarshal(msg.Payload(), &data); err != nil {
		log.Printf("❌ Invalid entrance payload: %v", err)
		return
	}

	err := h.EntranceExitUseCase.HandleEntrance(data.RFID, time.Unix(data.Timestamp, 0))
	if err != nil {
		log.Printf("❌ Error handling entrance: %v", err)
		return
	}

	client.Publish("parking/ack", 1, false, `{"status":"ok","event":"entrance"}`)
}

// Called when a car exits
func (h *Handler) OnExit(client mqtt.Client, msg mqtt.Message) {
	log.Printf("🏁 Received exit event on topic [%s]: %s", msg.Topic(), string(msg.Payload()))

	var data EntrancePayload
	if err := json.Unmarshal(msg.Payload(), &data); err != nil {
		log.Printf("❌ Invalid exit payload: %v", err)
		return
	}

	err := h.EntranceExitUseCase.HandleExit(data.RFID, time.Unix(data.Timestamp, 0))
	if err != nil {
		log.Printf("❌ Error handling exit: %v", err)
		return
	}

	client.Publish("parking/ack", 1, false, `{"status":"ok","event":"exit"}`)
}
