package http

import (
	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/config"
	"github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"gorm.io/gorm"
)

type App struct {
	Router *gin.Engine
	Config *config.Config
	DB     *gorm.DB
}

func NewApp(cfg *config.Config, db *gorm.DB, vehicleHandler *handler.VehicleHandler) *App {
	r := gin.Default()

	api := r.Group("/api/v1")
	{
		api.POST("/vehicles", vehicleHandler.Register)
		api.GET("/vehicles", vehicleHandler.List)
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
