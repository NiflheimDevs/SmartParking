package http

import (
	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/config"
	http_handler "github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"github.com/niflheimdevs/smartparking/internal/middleware"
)

type Handlers struct {
	Vehicle      *http_handler.VehicleHandler
	EntranceExit *http_handler.EntranceExitHandler
	ParkingSpot  *http_handler.ParkingSpotHandler
	User         *http_handler.UserHandler
	Gate         *http_handler.GateHandler
}

type Middlewares struct {
	JWT      *middleware.JWTMiddleware
	Ban      *middleware.IPBanMiddleware
	IPLogger *middleware.IPLoggerMiddleware
}

type HTTPApp struct {
	Router *gin.Engine
	Config *config.Config
}

func NewHttpApp(cfg *config.Config, handlers *Handlers, middlewares *Middlewares) *HTTPApp {
	r := gin.Default()
	r.Use(gin.Recovery())
	r.Use(cors.New(cors.Config{
		AllowOrigins:     []string{"https://smartparking.niflheimdevs.ir", "https://www.smartparking.niflheimdevs.ir", "http://localhost:8080"},
		AllowMethods:     []string{"GET", "POST", "PUT", "DELETE", "OPTIONS"},
		AllowHeaders:     []string{"Origin", "Content-Type", "Authorization"},
		AllowCredentials: true,
	}))
	r.Use(middlewares.IPLogger.IPLogger())
	r.Use(middlewares.Ban.CheckIPBan())

	protected := r.Group("/v1")
	protected.Use(middlewares.JWT.Validate())
	{
		vehicle_api := protected.Group("/vehicles")
		{
			vehicle_api.GET("/", handlers.Vehicle.List)
			vehicle_api.GET("/:id", handlers.Vehicle.Info)
			vehicle_api.POST("/", handlers.Vehicle.Register)
			vehicle_api.PUT("/:id", handlers.Vehicle.Update)
			vehicle_api.DELETE("/:id", handlers.Vehicle.Delete)
		}

		ee_api := protected.Group("/ee")
		{
			ee_api.GET("/", handlers.EntranceExit.List)
			ee_api.GET("/:id", handlers.EntranceExit.Info)
			ee_api.GET("/vehicle/:id", handlers.EntranceExit.VehicleLog)
		}

		ps_api := protected.Group("/ps")
		{
			ps_api.GET("/", handlers.ParkingSpot.GetStatus)
		}
		g_api := protected.Group("/gate")
		{
			g_api.POST("/control", handlers.Gate.Control)
		}
	}

	u_api := r.Group("/v1")
	{
		u_api.POST("/signup", handlers.User.Signup)
		u_api.POST("/login", handlers.User.Login)
	}

	return &HTTPApp{
		Router: r,
		Config: cfg,
	}
}

func (a *HTTPApp) Run() error {
	return a.Router.Run(a.Config.Server.Port)
}
