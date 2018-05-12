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

void PlayerControllerHuman::handleEvent( Command& command ){

    const double MAX_VEL_X=playerModel->getMaxVelX();
    const double MAX_VEL_Y=playerModel->getMaxVelY();

    //If a key was pressed
    if (command.key == CommandKey::KEY_DOWN) {
		
		switch(command.type){
			case CommandType::GO_UP:{
				this->pressingUP = true;
				playerModel->changeVelY(-MAX_VEL_Y);
				log->debug("PlayerControllerHuman: apretando arriba");
                break;
            }
            case CommandType::GO_DOWN: {
				this->pressingDOWN = true;
                playerModel->changeVelY(MAX_VEL_Y);
				log->debug("PlayerControllerHuman: apretando abajo");
                break;
            }
            case CommandType::GO_LEFT:{
				this->pressingLEFT = true;
				playerModel->changeVelX(-MAX_VEL_X);
				log->debug("PlayerControllerHuman: apretando izquierda");
                break;
            }
            case CommandType::GO_RIGHT:{
				this->pressingRIGHT = true;
				playerModel->changeVelX(MAX_VEL_X);
				log->debug("PlayerControllerHuman: apretando derecha");
                break;
            }
			case CommandType::SWEEP: {
				playerModel->sweep();
				log->debug("PlayerControllerHuman: apretando barrer");
				break;
			}
			case CommandType::SPRINT: {
				playerModel->sprint();
				log->debug("PlayerControllerHuman: apretando correr");
				break;
			}
			case CommandType::KICK: {
                if(playerModel->getHasControlOfTheBall()) {
                    playerModel->kick(world.getBall().getModel());
                }

				log->debug("PlayerControllerHuman: apretando patear");
				break;
			}
			case CommandType::CHANGE_PLAYER: {
				this->world.swap(this);
				log->debug("PlayerControllerHuman: cambiando el jugador controlado");
				break;
			}
			case CommandType::PASS:{
                if(playerModel->getHasControlOfTheBall()){
                    PlayerController* passController=world.getPlayerToPass(this);
                    playerModel->pass(passController->getModel(), world.getBall().getModel());
                    this->world.swap(this);
                }
				log->debug("PlayerControllerHuman: apretando pasar");
				break;
			}
			default: {
				// do Nothing
				Log::get_instance()->info("Unhandled command type");
				break;
			}
        }
    } else if(command.key == CommandKey::KEY_UP) { //If a key was released 
		if (command.type == CommandType::GO_UP && this->pressingUP) {
			this->pressingUP = false;
			playerModel->changeVelY(MAX_VEL_Y);
			log->debug("PlayerControllerHuman: soltando arriba");
		}
		else if (command.type == CommandType::GO_DOWN && this->pressingDOWN) {
			this->pressingDOWN = false;
			playerModel->changeVelY(-MAX_VEL_Y);
			log->debug("PlayerControllerHuman: soltando abajo");
		}
		else if (command.type == CommandType::GO_LEFT && this->pressingLEFT) {
			this->pressingLEFT = false;
			playerModel->changeVelX(MAX_VEL_X);
			log->debug("PlayerControllerHuman: soltando izquierda");
		}
		else if (command.type == CommandType::GO_RIGHT && this->pressingRIGHT) {
			this->pressingRIGHT = false;
			playerModel->changeVelX(-MAX_VEL_X);
			log->debug("PlayerControllerHuman: soltando derecha");
		}
		else if (command.type == CommandType::SPRINT) {
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