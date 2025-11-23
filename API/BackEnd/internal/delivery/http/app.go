package http

import (
	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/config"
	http_handler "github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"github.com/sirupsen/logrus"
	"gorm.io/gorm"
)

type App struct {
	Router *gin.Engine
	Config *config.Config
	DB     *gorm.DB
}

func NewHttpApp(cfg *config.Config, db *gorm.DB, vehicleHandler *http_handler.VehicleHandler, entranceexitHandler *http_handler.EntranceExitHandler) *App {
	r := gin.New()

	r.Use(gin.LoggerWithWriter(logrus.StandardLogger().Writer()))
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
