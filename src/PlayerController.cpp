//
// Created by federico on 11/04/18.
//

#include "PlayerController.h"

PlayerController::PlayerController(PlayerModel* model, PlayerView* view){
    playerModel=model;
    playerView=view;
}

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
