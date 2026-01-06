package mqtt_handler

import (
	"encoding/json"
	"log"

	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/niflheimdevs/smartparking/internal/usecase"
)

type SensorHandler struct {
	Client              mqtt.Client
	EntranceExitUseCase *usecase.EntranceExitUseCase
	ParkingSpotUseCase  *usecase.ParkingSpotUseCase
	VehicleUseCase      *usecase.VehicleUseCase
}

func NewSensorHandler(
	client mqtt.Client,
	eeuc *usecase.EntranceExitUseCase,
	psuc *usecase.ParkingSpotUseCase,
	vuc *usecase.VehicleUseCase,
) *SensorHandler {
	return &SensorHandler{
		Client:              client,
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
	OwnerName   string `json:"owner"`
	Error       string `json:"error"`
}

type ExitRespnsePayload struct {
	RFID  string `json:"rfid"`
	Price int64  `json:"price"`
	Error string `json:"error"`
}

type GatePayload struct {
	Gate  string `json:"gate"`
	State bool   `json:"state"`
}

func (h *SensorHandler) OnEntrance(msg mqtt.Message) {
	var p RFIDRequestPayload
	topic := "parking/entrance/response"

	if err := json.Unmarshal(msg.Payload(), &p); err != nil {
		log.Printf("Can't Parse JSON")
		resp := EnterResponsePayload{RFID: "", ParkingSpot: 0, Exist: false, Error: err.Error()}
		if b, mErr := json.Marshal(resp); mErr == nil {
			h.Publish(topic, 2, false, b)
		}
		return
	}

	resp := EnterResponsePayload{RFID: p.RFID}

	exists, err := h.VehicleUseCase.CheckRFID(p.RFID)
	if err != nil {
		log.Printf("Error in check RFID")
		resp.Error = err.Error()
		if b, mErr := json.Marshal(resp); mErr == nil {
			h.Publish(topic, 2, false, b)
		}
		return
	}

	if !exists {
		log.Printf("RFID doesn't EXIST")
		resp.Exist = false
		resp.Error = "RFID doesn't Exists"
		if b, mErr := json.Marshal(resp); mErr == nil {
			h.Publish(topic, 2, false, b)
		}
		return
	}

	vehicle, _ := h.VehicleUseCase.VehicleInfoRFID(p.RFID)

	spot, err := h.ParkingSpotUseCase.FindFree()
	if err != nil {
		log.Printf("No Free Space")
		resp.Error = err.Error()
		if b, mErr := json.Marshal(resp); mErr == nil {
			h.Publish(topic, 2, false, b)
		}
		return
	}

	err = h.EntranceExitUseCase.Enter(p.RFID)
	if err != nil {
		log.Printf("Error on Enter to parking")
		resp.Error = err.Error()
		if b, mErr := json.Marshal(resp); mErr == nil {
			h.Publish(topic, 2, false, b)
		}
		return
	}

	log.Printf("ALL IS GOOD")
	resp.Exist = true
	resp.ParkingSpot = int(spot.ID - 1)
	resp.OwnerName = vehicle.OwnerName
	resp.Error = ""
	if b, mErr := json.Marshal(resp); mErr == nil {
		h.Publish(topic, 2, false, b)
	}
}

func (h *SensorHandler) OnExit(msg mqtt.Message) {
	var p RFIDRequestPayload
	topic := "parking/exit/response"

	if err := json.Unmarshal(msg.Payload(), &p); err != nil {
		log.Printf("Can't Parse JSON")
		resp := ExitRespnsePayload{RFID: "", Price: 0, Error: err.Error()}
		if b, mErr := json.Marshal(resp); mErr == nil {
			h.Publish(topic, 2, false, b)
		}
		return
	}

	resp := ExitRespnsePayload{RFID: p.RFID}

	exists, err := h.VehicleUseCase.CheckRFID(p.RFID)
	if err != nil {
		log.Printf("Error in check RFID")
		resp.Error = err.Error()
		if b, mErr := json.Marshal(resp); mErr == nil {
			h.Publish(topic, 2, false, b)
		}
		return
	}

	if !exists {
		log.Printf("RFID doesn't exist")
		resp.Error = "RFID doesn't Exists"
		if b, mErr := json.Marshal(resp); mErr == nil {
			h.Publish(topic, 2, false, b)
		}
		return
	}

	price, err := h.EntranceExitUseCase.Exit(p.RFID)
	if err != nil {
		log.Printf("Error on exit")
		resp.Error = err.Error()
		if b, mErr := json.Marshal(resp); mErr == nil {
			h.Publish(topic, 2, false, b)
		}
		return
	}

	resp.Price = price
	resp.Error = ""
	if b, mErr := json.Marshal(resp); mErr == nil {
		h.Publish(topic, 2, false, b)
	}
}

func (h *SensorHandler) OnSpaceChange(msg mqtt.Message) {
	var p SpaceRequestPayload

	if err := json.Unmarshal(msg.Payload(), &p); err != nil {
		log.Printf("Can't Parse JSON")
		return
	}

	spotID, err := h.ParkingSpotUseCase.Update(p.SpaceNumber, p.IsOccupied)
	if err != nil {
		log.Printf("There is a problem in update parking space %s status", p.SpaceNumber)
		log.Print(err.Error())
		return
	}

	if p.IsOccupied == true {
		err = h.EntranceExitUseCase.ParkVehicle(spotID)
		if err != nil {
			log.Printf("There is a problem in update parking status of vehicle")
			log.Print(err.Error())
			return
		}
	}

	log.Printf("Parking Space %s updated succesfully", p.SpaceNumber)
}

func (h *SensorHandler) Gate(gate string, state bool) {
	topic := "parking/gate/control"
	payload := GatePayload{Gate: gate, State: state}
	if b, mErr := json.Marshal(payload); mErr == nil {
		h.Publish(topic, 2, false, b)
	} else {
		log.Println(mErr.Error())
	}
}

func (h *SensorHandler) Publish(topic string, qos byte, retained bool, payload interface{}) {
	token := h.Client.Publish(topic, qos, retained, payload)
	token.Wait()
}
