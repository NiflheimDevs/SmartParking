package logger

import (
	"log"
	"time"

	rotatelogs "github.com/lestrrat-go/file-rotatelogs"

	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
)

func SetupLogger() *zap.Logger {
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

	writeSyncer := zapcore.AddSync(writer)

	encoderCfg := zap.NewProductionEncoderConfig()
	encoderCfg.TimeKey = "time"
	encoderCfg.EncodeTime = zapcore.ISO8601TimeEncoder

	core := zapcore.NewCore(
		zapcore.NewJSONEncoder(encoderCfg),
		writeSyncer,
		zapcore.DebugLevel,
	)

	logger := zap.New(core, zap.AddCaller(), zap.AddStacktrace(zapcore.ErrorLevel))
	zap.ReplaceGlobals(logger)
	defer logger.Sync()

	log.SetOutput(zap.NewStdLog(logger).Writer())
	return logger
}
