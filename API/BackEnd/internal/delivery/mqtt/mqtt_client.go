package mqtt

import (
	"log"

	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/niflheimdevs/smartparking/internal/config"
)

type MQTTClient struct {
	client mqtt.Client
}

func InitMQTT(cfg *config.Config) *MQTTClient {
	opts := mqtt.NewClientOptions().
		AddBroker(cfg.MQTT.Broker).
		SetClientID(cfg.MQTT.ClientID)

	client := mqtt.NewClient(opts)
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Fatalf("MQTT connection failed: %v", token.Error())
	}

	return &MQTTClient{client: client}
}

func (m *MQTTClient) Listen() {
	m.client.Subscribe("parking/gate/entrance", 0, func(c mqtt.Client, msg mqtt.Message) {
		log.Println("Entrance detected:", string(msg.Payload()))
	})
}
