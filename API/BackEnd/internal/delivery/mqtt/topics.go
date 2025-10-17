package mqtt

import (
	"log"

	mqtt "github.com/eclipse/paho.mqtt.golang"
)

const (
	TopicEntrance = "parking/entrance"
	TopicExit     = "parking/exit"
)

func (m *MQTTClient) SubscribeTopics() {
	topics := map[string]mqtt.MessageHandler{
		TopicEntrance: m.Handler.OnEntrance,
		TopicExit:     m.Handler.OnExit,
	}

	for topic, h := range topics {
		if token := m.client.Subscribe(topic, 1, h); token.Wait() && token.Error() != nil {
			log.Printf("❌ Failed to subscribe to %s: %v", topic, token.Error())
		} else {
			log.Printf("📡 Subscribed to topic: %s", topic)
		}
	}
}
