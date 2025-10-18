package http

import (
	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/config"
	http_handler "github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"gorm.io/gorm"
)

type App struct {
	Router *gin.Engine
	Config *config.Config
	DB     *gorm.DB
}

func NewHttpApp(cfg *config.Config, db *gorm.DB, vehicleHandler *http_handler.VehicleHandler, entranceexitHandler *http_handler.EntranceExitHandler) *App {
	r := gin.Default()

	vehicle_api := r.Group("/api/v1/vehicles")
	{
		vehicle_api.POST("/", vehicleHandler.Register)
		vehicle_api.GET("/", vehicleHandler.List)
		vehicle_api.PUT("/:id", vehicleHandler.Update)
	}

	ee_api := r.Group("/api/v1/ee")
	{
		ee_api.GET("/", entranceexitHandler.List)
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
