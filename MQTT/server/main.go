package main

import (
	"fmt"
	"time"

	mqtt "github.com/eclipse/paho.mqtt.golang"
)

func main() {
	// MQTT broker options
	opts := mqtt.NewClientOptions().AddBroker("tcp://localhost:1883")
	opts.SetClientID("go_mqtt_client")

	// Connect handler
	opts.OnConnect = func(c mqtt.Client) {
		fmt.Println("Connected to MQTT broker")
	}

	client := mqtt.NewClient(opts)
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		panic(token.Error())
	}

	// Subscribe to a topic
	client.Subscribe("home/sensors", 0, func(client mqtt.Client, msg mqtt.Message) {
		fmt.Printf("Received message: %s from topic: %s\n", msg.Payload(), msg.Topic())
	})

	// Publish messages
	for {
		text := fmt.Sprintf("Hello at %s", time.Now().Format(time.RFC3339))
		client.Publish("home/sensors", 0, false, text)
		time.Sleep(5 * time.Second)
	}
}
