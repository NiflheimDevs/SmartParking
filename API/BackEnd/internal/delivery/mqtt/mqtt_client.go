package mqtt

import (
	"log"

	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/niflheimdevs/smartparking/internal/config"
	mqtt_handler "github.com/niflheimdevs/smartparking/internal/delivery/mqtt/handler"
	"gorm.io/gorm"
)

type MQTTClient struct {
	client  mqtt.Client
	Handler *mqtt_handler.SensorHandler
	Config  *config.Config
	DB      *gorm.DB
}

func InitMQTTClient(cfg *config.Config) mqtt.Client {
	opts := mqtt.NewClientOptions().
		AddBroker(cfg.MQTT.Broker).
		SetClientID(cfg.MQTT.ClientID)

	client := mqtt.NewClient(opts)
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Fatalf("MQTT connection failed: %v", token.Error())
	}

	return client
}

func InitMQTT(client mqtt.Client, cfg *config.Config, handler *mqtt_handler.SensorHandler) *MQTTClient {
	return &MQTTClient{client: client, Handler: handler, Config: cfg}
}

func (m *MQTTClient) Listen() {
	m.client.Subscribe("parking/entrance", 0, func(c mqtt.Client, msg mqtt.Message) {
		if m.Handler != nil {
			m.Handler.OnEntrance(msg)
			return
		}
		log.Println("Entrance detected (no handler):", string(msg.Payload()))
	})

	m.client.Subscribe("parking/exit", 0, func(c mqtt.Client, msg mqtt.Message) {
		if m.Handler != nil {
			m.Handler.OnExit(msg)
			return
		}
		log.Println("Exit detected (no handler):", string(msg.Payload()))
	})

	m.client.Subscribe("parking/space", 0, func(c mqtt.Client, msg mqtt.Message) {
		if m.Handler != nil {
			m.Handler.OnSpaceChange(msg)
			return
		}
		log.Println("Space change detected (no handler):", string(msg.Payload()))
	})
}

func (m *MQTTClient) Publish(topic string, qos byte, retained bool, payload interface{}) {
	token := m.client.Publish(topic, qos, retained, payload)
	token.Wait()
}
