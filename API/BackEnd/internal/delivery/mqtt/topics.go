package mqtt

import (
	"log"

	mqtt "github.com/eclipse/paho.mqtt.golang"
)

// Topic constants
const (
	TopicEntrance = "parking/entrance"
	TopicExit     = "parking/exit"
)

// SubscribeTopics connects handlers to topics.
func (m *MQTTClient) SubscribeTopics(handler *Handler) {
	topics := map[string]mqtt.MessageHandler{
		TopicEntrance: handler.OnEntrance,
		TopicExit:     handler.OnExit,
	}

	for topic, h := range topics {
		if token := m.client.Subscribe(topic, 1, h); token.Wait() && token.Error() != nil {
			log.Printf("❌ Failed to subscribe to %s: %v", topic, token.Error())
		} else {
			log.Printf("📡 Subscribed to topic: %s", topic)
		}
	}
}
