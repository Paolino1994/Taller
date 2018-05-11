#include "PlayerControllerHuman.h"
#include <math.h>
#include <iostream>
using namespace std;

PlayerControllerHuman::PlayerControllerHuman(PlayerModel * model, PlayerView * view, World& world):
	PlayerController(model, view),
	world(world),
	pressingUP(false),
	pressingDOWN(false),
	pressingLEFT(false),
	pressingRIGHT(false),
	log(Log::get_instance())
{
}

void PlayerControllerHuman::handleEvent( SDL_Event& e )
{

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
				playerModel->changeVelY(-MAX_VEL_Y);
				log->debug("PlayerControllerHuman: apretando arriba");
                break;
            }
            case SDLK_DOWN: {
				this->pressingDOWN = true;
                playerModel->changeVelY(MAX_VEL_Y);
				log->debug("PlayerControllerHuman: apretando abajo");
                break;
            }
            case SDLK_LEFT:{
				this->pressingLEFT = true;
				playerModel->changeVelX(-MAX_VEL_X);
				log->debug("PlayerControllerHuman: apretando izquierda");
                break;
            }
            case SDLK_RIGHT:{
				this->pressingRIGHT = true;
				playerModel->changeVelX(MAX_VEL_X);
				log->debug("PlayerControllerHuman: apretando derecha");
                break;
            }
			case SDLK_a: {
				playerModel->sweep();
				log->debug("PlayerControllerHuman: apretando barrer");
				break;
			}
			case SDLK_SPACE: {
				playerModel->sprint();
				log->debug("PlayerControllerHuman: apretando correr");
				break;
			}
			case SDLK_s: {
				playerModel->kick(world.getBall().getModel());

				log->debug("PlayerControllerHuman: apretando patear");
				break;
			}
			case SDLK_q: {
				this->world.swap(this);
				log->debug("PlayerControllerHuman: cambiando el jugador controlado");
				break;
			}
			case SDLK_d:{
				PlayerController* passController=world.getPlayerToPass(this);
				playerModel->pass(passController->getModel(), world.getBall().getModel());
				log->debug("PlayerControllerHuman: apretando pasar");
			}
        }
    }

	//If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity

		if (e.key.keysym.sym == SDLK_UP && this->pressingUP) {
			this->pressingUP = false;
			playerModel->changeVelY(MAX_VEL_Y);
			log->debug("PlayerControllerHuman: soltando arriba");
		}
		else if (e.key.keysym.sym == SDLK_DOWN && this->pressingDOWN) {
			this->pressingDOWN = false;
			playerModel->changeVelY(-MAX_VEL_Y);
			log->debug("PlayerControllerHuman: soltando abajo");
		}
		else if (e.key.keysym.sym == SDLK_LEFT && this->pressingLEFT) {
			this->pressingLEFT = false;
			playerModel->changeVelX(MAX_VEL_X);
			log->debug("PlayerControllerHuman: soltando izquierda");
		}
		else if (e.key.keysym.sym == SDLK_RIGHT && this->pressingRIGHT) {
			this->pressingRIGHT = false;
			playerModel->changeVelX(-MAX_VEL_X);
			log->debug("PlayerControllerHuman: soltando derecha");
		}
		else if (e.key.keysym.sym == SDLK_SPACE) {
			playerModel->stopSprinting();
			log->debug("PlayerControllerHuman: soltando correr");
		}


    }

    //Bastabte villero
    playerModel->changeBallState(world.getBall().getModel());


    // se normaliza la velocidad
	float factor = 0.70710678;

    if(this->pressingUP && this->pressingLEFT){
    	log->debug("PlayerControllerHuman: up-left");
    	playerModel->setVelY(-playerModel->getMaxVelY() * factor);
    	playerModel->setVelX(-playerModel->getMaxVelX() * factor);
    }

    if(this->pressingUP && this->pressingRIGHT){
    	log->debug("PlayerControllerHuman: up-right");
		playerModel->setVelY(-playerModel->getMaxVelY() * factor);
		playerModel->setVelX(playerModel->getMaxVelX()  * factor);
	}

    if(this->pressingLEFT && this->pressingDOWN){
    	log->debug("PlayerControllerHuman: left-down");
		playerModel->setVelX(-playerModel->getMaxVelX() * factor);
		playerModel->setVelY(playerModel->getMaxVelY()  * factor);
	}

    if(this->pressingRIGHT && this->pressingDOWN){
    	log->debug("PlayerControllerHuman: right-down");
		playerModel->setVelX(playerModel->getMaxVelX() * factor);
		playerModel->setVelY(playerModel->getMaxVelY() * factor);
	}

    if(this->pressingUP && !this->pressingLEFT && !this->pressingRIGHT){
		playerModel->setVelY(-MAX_VEL_Y);
		playerModel->setVelX(0);
    }

    if(this->pressingDOWN && !this->pressingLEFT && !this->pressingRIGHT){
		playerModel->setVelY(MAX_VEL_Y);
		playerModel->setVelX(0);
	}

    if(this->pressingLEFT && !this->pressingUP && !this->pressingDOWN){
		playerModel->setVelX(-MAX_VEL_X);
		playerModel->setVelY(0);
    }

    if(this->pressingRIGHT && !this->pressingUP && !this->pressingDOWN){
		playerModel->setVelX(MAX_VEL_X);
		playerModel->setVelY(0);
	}

}

void PlayerControllerHuman::swap(PlayerController * otherController)
{
	PlayerController::swap(otherController);

	//Reset de lo que tecleabamos porque no aplica al nuevo jugador
	this->pressingUP = false;
	this->pressingDOWN = false;
	this->pressingLEFT = false;
	this->pressingRIGHT = false;

	//Reset del movimiento del jugador que tomamos control
	this->playerModel->changeVelX(-this->playerModel->getVelX());
	this->playerModel->changeVelY(-this->playerModel->getVelY());
	log->debug("PlayerControllerHuman: cambiando jugador");
}

bool PlayerControllerHuman::isControllable() {
	return false;
}