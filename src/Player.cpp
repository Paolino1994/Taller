#include "Player.h"

#include "SDL.h"
#include <cmath>
#include <iostream>

Player::Player(std::map<const std::string, Animation>& animationMapper, const player_data_t player_data, double initial_x, double initial_y):
    Entity(initial_x, initial_y),
    velX(0),
    velY(0),
    state(STILL),
	angle(0.0),
    MAX_VEL_X(player_data.X_VELOCITY),
    MAX_VEL_Y(player_data.Y_VELOCITY)
{
    this->mAnimations.reserve(_LENGTH_);
    for(int i=0; i<PlayerState::_LENGTH_; i++){
        // copias de animations
        this->mAnimations.push_back(animationMapper.at(player_data.sprite_ids[i]));
    }
}

Player::~Player()
{
}

int Player::getWidth()
{
    return mAnimations[this->state].getDestinationWidth();
}

int Player::getHeight()
{
    return mAnimations[this->state].getDestinationHeight();
}

void Player::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP:{
				velY -= MAX_VEL_Y;
				//flipmode = SDL_FLIP_NONE;
                break;
            }
			case SDLK_DOWN: {
				velY += MAX_VEL_Y; 
				//flipmode = static_cast<SDL_RendererFlip>(flipmode | SDL_FLIP_VERTICAL); // ver como aplicar (en general) a todos los setFlipModes
				break;
			}
            case SDLK_LEFT:{
                velX -= MAX_VEL_X;
                //flipmode = SDL_FLIP_HORIZONTAL; //ver si tiene sentido cuando se use con el angulo
                break;
            }
            case SDLK_RIGHT:{
                velX += MAX_VEL_X;
                //flipmode = SDL_FLIP_NONE;
                break;
            }
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: velY += MAX_VEL_Y; break;
            case SDLK_DOWN: velY -= MAX_VEL_Y; break;
            case SDLK_LEFT: velX += MAX_VEL_X; break;
            case SDLK_RIGHT: velX -= MAX_VEL_X; break;
        }
    }
}

void Player::update(double dt, int x_limit, int y_limit){
    using namespace std;
    PlayerState old_state = this->state;
    // Actualizar x:
    x += velX * dt;
	// Actualizar y:
    y += velY * dt;

    
    if((y + this->getHeight()) > y_limit ) { //limite de abajo 
        y = y_limit - this->getHeight();
	}
	else if (this->y < 0) { // limite de arriba
		this->y = 0;
	}

	if ((x + this->getWidth()) > x_limit) { //limite de abajo 
		x = x_limit - this->getWidth();
	}
	else if (this->x < 0) { // limite de arriba
		this->x = 0;
	}
    
	if (velX == 0.0 && velY == 0.0) {
		this->state = STILL;
		// mantenemos el angulo anterior
	}
	else {
		this->state = RUNNING;
		// angle con 0 apunta para arriba, 180 abajo, 360 arriba, lo pasado de 360 o 0 lo modulea SDL2
		this->angle = (atan2(this->velY, this->velX) * 180 / M_PI) + 90;
	}
    
    if (old_state != this->state) mAnimations[this->state].reset();
    mAnimations[this->state].update(dt);
}

void Player::render(int screen_x, int screen_y)
{
	mAnimations[this->state].render(screen_x, screen_y, this->angle);
}

