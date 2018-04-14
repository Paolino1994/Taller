//
// Created by federico on 11/04/18.
//

#include "PlayerController.h"

void PlayerController::handleEvent( SDL_Event& e )
{
    //double velX=playerModel->getVelX();
    //double velY=playerModel->getVelY();
    const double MAX_VEL_X=playerModel->getMaxVelX();
    const double MAX_VEL_Y=playerModel->getMaxVelY();

    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP:{
				this->pressingUP = true;
                //velY -= MAX_VEL_Y;
                playerModel->changeVelY(-MAX_VEL_Y);
                break;
            }
            case SDLK_DOWN: {
				this->pressingDOWN = true;
                //velY += MAX_VEL_Y;
                playerModel->changeVelY(MAX_VEL_Y);
                break;
            }
            case SDLK_LEFT:{
				this->pressingLEFT = true;
                //velX -= MAX_VEL_X;
                playerModel->changeVelX(-MAX_VEL_X);
                break;
            }
            case SDLK_RIGHT:{
				this->pressingRIGHT = true;
                //velX += MAX_VEL_X;
                playerModel->changeVelX(MAX_VEL_X);
                break;
            }
			case SDLK_SPACE: {
				playerModel->sweep();
				break;
			}
			case SDLK_r: {
				playerModel->sprint();
				break;
			}
        }
    }
	
	//If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity

		if (e.key.keysym.sym == SDLK_UP && this->pressingUP) {
			//velY += MAX_VEL_Y;
			playerModel->changeVelY(MAX_VEL_Y);
		}
		else if (e.key.keysym.sym == SDLK_DOWN && this->pressingDOWN) {
			//velY -= MAX_VEL_Y;
			playerModel->changeVelY(-MAX_VEL_Y);
		}
		else if (e.key.keysym.sym == SDLK_LEFT && this->pressingLEFT) {
			//velX += MAX_VEL_X;
			playerModel->changeVelX(MAX_VEL_X);
		}
		else if (e.key.keysym.sym == SDLK_RIGHT && this->pressingRIGHT) {
			//velX -= MAX_VEL_X;
			playerModel->changeVelX(-MAX_VEL_X);
		}
		else if (e.key.keysym.sym == SDLK_r) {
			playerModel->stopSprinting();
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

	//Reset de lo que tecleabamos porque no aplica al nuevo jugador
	this->pressingUP = false;
	this->pressingDOWN = false;
	this->pressingLEFT = false;
	this->pressingRIGHT = false;
}
