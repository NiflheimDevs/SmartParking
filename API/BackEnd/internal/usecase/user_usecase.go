package usecase

import (
	"github.com/niflheimdevs/smartparking/internal/domain"
	"golang.org/x/crypto/bcrypt"
)

type UserRepository interface {
	Create(user *domain.User) error
	GetByID(id uint) (*domain.User, error)
	GetByUsername(username string) (*domain.User, error)
}

type UserUseCase struct {
	repo UserRepository
	jwt  *JWT
}

func NewUserUseCase(repo UserRepository, jwt *JWT) *UserUseCase {
	return &UserUseCase{
		repo: repo,
		jwt:  jwt,
	}
}

func (uc *UserUseCase) Signup(username, password string) error {
	pass, err := bcrypt.GenerateFromPassword([]byte(password), bcrypt.DefaultCost)
	if err != nil {
		return err
	}
	user := domain.User{
		Username: username,
		Password: pass,
	}
	err = uc.repo.Create(&user)
	return err
}

func (uc *UserUseCase) Login(username, password string) (string, error) {
	user, err := uc.repo.GetByUsername(username)
	if err != nil {
		return "", err
	}
	if err = bcrypt.CompareHashAndPassword(user.Password, []byte(password)); err != nil {
		return "", err
	}
	token, err := uc.jwt.GenerateToken(user.ID)
	if err != nil {
		return "", err
	}
	return token, nil
}
