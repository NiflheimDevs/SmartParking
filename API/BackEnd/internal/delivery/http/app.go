package http

import (
	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"github.com/niflheimdevs/smartparking/internal/config"
	http_handler "github.com/niflheimdevs/smartparking/internal/delivery/http/handler"
	"github.com/niflheimdevs/smartparking/internal/middleware"
	"gorm.io/gorm"
)

type Handlers struct {
	Vehicle      *http_handler.VehicleHandler
	EntranceExit *http_handler.EntranceExitHandler
	ParkingSpot  *http_handler.ParkingSpotHandler
	User         *http_handler.UserHandler
}

type Middlewares struct {
	JWT *middleware.JWTMiddleware
}

func NewHandlers(
	v *http_handler.VehicleHandler,
	ee *http_handler.EntranceExitHandler,
	ps *http_handler.ParkingSpotHandler,
	u *http_handler.UserHandler,
) *Handlers {
	return &Handlers{
		Vehicle:      v,
		EntranceExit: ee,
		ParkingSpot:  ps,
		User:         u,
	}
}

func NewMiddlewares(
	jwt *middleware.JWTMiddleware,
) *Middlewares {
	return &Middlewares{
		JWT: jwt,
	}
}

type App struct {
	Router *gin.Engine
	Config *config.Config
	DB     *gorm.DB
}

func NewHttpApp(cfg *config.Config, db *gorm.DB, handlers *Handlers, middlewares *Middlewares) *App {
	r := gin.Default()
	r.Use(gin.Recovery())
	r.Use(cors.New(cors.Config{
		AllowOrigins:     []string{"*"}, // or your frontend URL
		AllowMethods:     []string{"GET", "POST", "PUT", "DELETE", "OPTIONS"},
		AllowHeaders:     []string{"Origin", "Content-Type", "Authorization"},
		AllowCredentials: true,
	}))

	protected := r.Group("/v1")
	//protected.Use(middlewares.JWT.Validate())
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
	}

	u_api := r.Group("/v1")
	{
		u_api.POST("/signup", handlers.User.Signup)
		u_api.POST("/login", handlers.User.Login)
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
