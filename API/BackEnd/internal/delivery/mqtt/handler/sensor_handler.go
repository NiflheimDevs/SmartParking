package mqtt_handler

import (
	"encoding/json"
	"log"

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
	topic := "parking/entrance/response"

	if err := json.Unmarshal(msg.Payload(), &p); err != nil {
		log.Printf("Can't Parse JSON")
		resp := ResponsePayload{RFID: "", ParkingSpot: 0, Exist: false, Error: err.Error()}
		if b, mErr := json.Marshal(resp); mErr == nil {
			client.Publish(topic, 0, false, b)
		}
		return
	}

	resp := ResponsePayload{RFID: p.RFID}

	exists, err := h.VehicleUseCase.CheckRFID(p.RFID)
	if err != nil {
		log.Printf("Error in check RFID")
		resp.Error = err.Error()
		if b, mErr := json.Marshal(resp); mErr == nil {
			client.Publish(topic, 0, false, b)
		}
		return
	}

	if !exists {
		log.Printf("RFID doesn't EXIST")
		resp.Exist = false
		resp.Error = "RFID doesn't Exists"
		if b, mErr := json.Marshal(resp); mErr == nil {
			client.Publish(topic, 0, false, b)
		}
		return
	}

	spot, err := h.ParkingSpotUseCase.FindFree()
	if err != nil {
		log.Printf("No Free Space")
		resp.Error = err.Error()
		if b, mErr := json.Marshal(resp); mErr == nil {
			client.Publish(topic, 0, false, b)
		}
		return
	}

	err = h.EntranceExitUseCase.Enter(p.RFID)
	if err != nil {
		log.Printf("Error on Enter to parking")
		resp.Error = err.Error()
		if b, mErr := json.Marshal(resp); mErr == nil {
			client.Publish(topic, 0, false, b)
		}
		return
	}

	log.Printf("ALL IS GOOD")
	resp.Exist = true
	resp.ParkingSpot = int(spot.ID)
	resp.Error = ""
	if b, mErr := json.Marshal(resp); mErr == nil {
		client.Publish(topic, 0, false, b)
	}
}

func (h *SensorHandler) OnExit(client mqtt.Client, msg mqtt.Message) {
	var p RequestPayload
	topic := "parking/exit/response"

	if err := json.Unmarshal(msg.Payload(), &p); err != nil {
		resp := ResponsePayload{RFID: "", ParkingSpot: 0, Exist: false, Error: err.Error()}
		if b, mErr := json.Marshal(resp); mErr == nil {
			client.Publish(topic, 0, false, b)
		}
		return
	}

	// Exit handling not implemented yet in usecase layer.
	resp := ResponsePayload{RFID: p.RFID, ParkingSpot: 0, Exist: false, Error: "exit not implemented"}
	if b, mErr := json.Marshal(resp); mErr == nil {
		client.Publish(topic, 0, false, b)
	}
}
