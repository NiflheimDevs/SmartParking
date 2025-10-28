package mqtt_handler

import (
	"encoding/json"

	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type SensorHandler struct {
	EntranceExitUseCase *usecase.EntranceExitUseCase
	ParkingSpotUseCase  *usecase.ParkingSpotUseCase
	VehicleUseCase      *usecase.VehicleUseCase
}

func NewSensorHandler(
	eeuc *usecase.EntranceExitUseCase,
	psuc *usecase.ParkingSpotUseCase,
	vuc *usecase.VehicleUseCase,
) *SensorHandler {
	return &SensorHandler{
		EntranceExitUseCase: eeuc,
		ParkingSpotUseCase:  psuc,
		VehicleUseCase:      vuc,
	}
}

type RequestPayload struct {
	RFID string `json:"rfid"`
}

type ResponsePayload struct {
	RFID        string `json:"rfid"`
	ParkingSpot int    `json:"parking_spot"`
	Exist       bool   `json:"exist"`
	Error       string `json:"error"`
}

func (h *SensorHandler) OnEntrance(client mqtt.Client, msg mqtt.Message) {
	var p RequestPayload
	resp := ResponsePayload{
		RFID: p.RFID,
	}
	topic := "parking/entrance/response"

	if err := json.Unmarshal(msg.Payload(), &p); err != nil {
		resp.Error = err.Error()
		client.Publish(topic, 0, false, resp)
		return
	}

	exists, err := h.VehicleUseCase.CheckRFID(p.RFID)
	if err != nil {
		resp.Error = err.Error()
		client.Publish(topic, 0, false, resp)
		return
	}

	if !exists {
		resp.Exist = false
		client.Publish(topic, 0, false, resp)
		return
	}

	spot, err := h.ParkingSpotUseCase.FindFree()

	if err != nil {
		resp.Error = err.Error()
		client.Publish(topic, 0, false, resp)
		return
	}

	err = h.EntranceExitUseCase.Enter(p.RFID)

	if err != nil {
		resp.Error = err.Error()
		client.Publish(topic, 0, false, resp)
		return
	}

	resp.Exist = true
	resp.ParkingSpot = int(spot.ID)
	resp.Error = ""
	client.Publish(topic, 0, false, resp)
}

func (h *SensorHandler) OnExit(client mqtt.Client, msg mqtt.Message) {

}
