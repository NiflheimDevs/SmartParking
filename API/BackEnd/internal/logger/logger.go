package logger

import (
	"log"
	"time"

	rotatelogs "github.com/lestrrat-go/file-rotatelogs"
	"github.com/sirupsen/logrus"
)

func SetupLogger() {
	baseLogPath := "/app/logs/app"

	writer, err := rotatelogs.New(
		baseLogPath+"-%Y-%m-%d.log",
		rotatelogs.WithLinkName("/app/logs/app.log"),
		rotatelogs.WithRotationTime(24*time.Hour),
		rotatelogs.WithMaxAge(30*24*time.Hour),
	)
	if err != nil {
		log.Fatalf("failed to create log rotator: %v", err)
	}

	logrus.SetOutput(writer)
	logrus.SetFormatter(&logrus.TextFormatter{
		FullTimestamp:   true,
		TimestampFormat: "2006-01-02 15:04:05",
	})

	log.SetOutput(writer)
}
