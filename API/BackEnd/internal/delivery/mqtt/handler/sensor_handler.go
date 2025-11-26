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

type RFIDRequestPayload struct {
	RFID string `json:"rfid"`
}

type SpaceRequestPayload struct {
	SpaceNumber string `json:"space_number"`
	IsOccupied  bool   `json:"is_occupied"`
}

type EnterResponsePayload struct {
	RFID        string `json:"rfid"`
	ParkingSpot int    `json:"parking_spot"`
	Exist       bool   `json:"exist"`
	Error       string `json:"error"`
}

type ExitRespnsePayload struct {
	RFID  string `json:"rfid"`
	Price int64  `json:"price"`
	Error string `json:"error"`
}

func (h *SensorHandler) OnEntrance(client mqtt.Client, msg mqtt.Message) {
	var p RFIDRequestPayload
	topic := "parking/entrance/response"

	if err := json.Unmarshal(msg.Payload(), &p); err != nil {
		log.Printf("Can't Parse JSON")
		resp := EnterResponsePayload{RFID: "", ParkingSpot: 0, Exist: false, Error: err.Error()}
		if b, mErr := json.Marshal(resp); mErr == nil {
			client.Publish(topic, 0, false, b)
		}
		return
	}

	resp := EnterResponsePayload{RFID: p.RFID}

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
	var p RFIDRequestPayload
	topic := "parking/exit/response"

	if err := json.Unmarshal(msg.Payload(), &p); err != nil {
		log.Printf("Can't Parse JSON")
		resp := ExitRespnsePayload{RFID: "", Price: 0, Error: err.Error()}
		if b, mErr := json.Marshal(resp); mErr == nil {
			client.Publish(topic, 0, false, b)
		}
		return
	}

	resp := ExitRespnsePayload{RFID: p.RFID}

	exists, err := h.VehicleUseCase.CheckRFID(p.RFID)
	if err != nil || !exists {
		log.Printf("Error in check RFID or it doesn't exists")
		resp.Error = err.Error()
		if b, mErr := json.Marshal(resp); mErr == nil {
			client.Publish(topic, 0, false, b)
		}
		return
	}

	price, err := h.EntranceExitUseCase.Exit(p.RFID)
	if err != nil {
		log.Printf("Error on exit")
		resp.Error = err.Error()
		if b, mErr := json.Marshal(resp); mErr == nil {
			client.Publish(topic, 0, false, b)
		}
		return
	}

	resp.Price = price
	resp.Error = ""
	if b, mErr := json.Marshal(resp); mErr == nil {
		client.Publish(topic, 0, false, b)
	}
}

func (h *SensorHandler) OnSpaceChange(client mqtt.Client, msg mqtt.Message) {
	var p SpaceRequestPayload

	if err := json.Unmarshal(msg.Payload(), &p); err != nil {
		log.Printf("Can't Parse JSON")
		return
	}

	spotID, err := h.ParkingSpotUseCase.Update(p.SpaceNumber, p.IsOccupied)
	if err != nil {
		log.Printf("There is a problem in update parking space %s status", p.SpaceNumber)
		return
	}

	err = h.EntranceExitUseCase.ParkVehicle(spotID)
	if err != nil {
		log.Printf("There is a problem in update parking status of vehicle")
		return
	}

	log.Printf("Parking Space %s updated succesfully", p.SpaceNumber)
}
