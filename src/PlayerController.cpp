//
// Created by federico on 11/04/18.
//

#include "PlayerController.h"

void PlayerController::handleEvent( SDL_Event& e )
{
    double velX=playerModel->getVelX();
    double velY=playerModel->getVelY();
    double MAX_VEL_X=playerModel->getMaxVelX();
    double MAX_VEL_Y=playerModel->getMaxVelY();

    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP:{
                velY -= MAX_VEL_Y;
                playerModel->setVelY(velY);
                //flipmode = SDL_FLIP_NONE;
                break;
            }
            case SDLK_DOWN: {
                velY += MAX_VEL_Y;
                playerModel->setVelY(velY);
                //flipmode = static_cast<SDL_RendererFlip>(flipmode | SDL_FLIP_VERTICAL); // ver como aplicar (en general) a todos los setFlipModes
                break;
            }
            case SDLK_LEFT:{
                velX -= MAX_VEL_X;
                playerModel->setVelX(velX);
                //flipmode = SDL_FLIP_HORIZONTAL; //ver si tiene sentido cuando se use con el angulo
                break;
            }
            case SDLK_RIGHT:{
                velX += MAX_VEL_X;
                playerModel->setVelX(velX);
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
            case SDLK_UP:
                velY += MAX_VEL_Y;
                playerModel->setVelY(velY);
                break;
            case SDLK_DOWN:
                velY -= MAX_VEL_Y;
                playerModel->setVelY(velY);
                break;
            case SDLK_LEFT:
                velX += MAX_VEL_X;
                playerModel->setVelX(velX);
                break;
            case SDLK_RIGHT:
                velX -= MAX_VEL_X;
                playerModel->setVelX(velX);
                break;
        }
    }
}

void PlayerController::update(double dt, int x_limit, int y_limit)
{
	this->playerModel->update(dt, x_limit, y_limit);
	this->playerView->update(dt);
}

void PlayerController::swap(PlayerController * otherController)
{
	auto temp = this->playerModel;
	this->playerModel = otherController->playerModel;
	otherController->playerModel = temp;

	auto tempView = this->playerView;
	this->playerView = otherController->playerView;
	otherController->playerView = tempView;

	//TEMPORAL, corregir
	//reset por las dudas los movimientos TEMPORAL
	//va a depender del estado del jugador, por ejemplo si estoy barriendo esto no sirve
	otherController->playerModel->setVelY(0);
	otherController->playerModel->setVelX(0);
	this->playerModel->setVelY(0);
	this->playerModel->setVelX(0);
}
