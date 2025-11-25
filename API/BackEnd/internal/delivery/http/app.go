package http

import (
	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/config"
	http_handler "github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"github.com/niflheimdevs/smartparking/internal/logger"
	"go.uber.org/zap"
	"gorm.io/gorm"
)

type App struct {
	Router *gin.Engine
	Config *config.Config
	DB     *gorm.DB
}

func NewHttpApp(cfg *config.Config, db *gorm.DB, vehicleHandler *http_handler.VehicleHandler, entranceexitHandler *http_handler.EntranceExitHandler) *App {
	logger := logger.SetupLogger()

	r := gin.Default()

	gin.DefaultWriter = zap.NewStdLog(logger).Writer()
	gin.DefaultErrorWriter = zap.NewStdLog(logger).Writer()
	r.Use(gin.LoggerWithWriter(gin.DefaultWriter))
	r.Use(gin.RecoveryWithWriter(gin.DefaultErrorWriter))
	r.Use(gin.Recovery())

	vehicle_api := r.Group("/v1/vehicles")
	{
		vehicle_api.GET("/", vehicleHandler.List)
		vehicle_api.GET("/:id", vehicleHandler.Info)
		vehicle_api.POST("/", vehicleHandler.Register)
		vehicle_api.PUT("/:id", vehicleHandler.Update)
		vehicle_api.DELETE("/:id", vehicleHandler.Delete)
	}

	ee_api := r.Group("/v1/ee")
	{
		ee_api.GET("/", entranceexitHandler.List)
		ee_api.GET("/:id", entranceexitHandler.Info)
		ee_api.GET("/vehicle/:id", entranceexitHandler.VehicleLog)
	}

	return &App{
		Router: r,
		Config: cfg,
		DB:     db,
	}
}

func (a *App) Run() error {
	return a.Router.Run(a.Config.Server.Port)
}
