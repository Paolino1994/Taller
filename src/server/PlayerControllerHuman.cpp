#include "PlayerControllerHuman.h"
#include "../common/Request.h"
#include "../common/Log.h"
#include <math.h>
#include <iostream>
using namespace std;

PlayerControllerHuman::PlayerControllerHuman(PlayerModel * model, PlayerView * view, World& world, User_ID userId):
	PlayerController(model, view),
	world(world),
	userId(userId),
	pressingUP(false),
	pressingDOWN(false),
	pressingLEFT(false),
	pressingRIGHT(false),
	log(Log::get_instance())
{
	this->getModel()->setIsControlledByHuman(true);
}

PlayerControllerHuman::PlayerControllerHuman(PlayerController * other, World & world, User_ID userId):
	PlayerController(other),
	world(world),
	userId(userId),
	pressingUP(false),
	pressingDOWN(false),
	pressingLEFT(false),
	pressingRIGHT(false),
	log(Log::get_instance())
{
	this->getModel()->setIsControlledByHuman(true);
}

void PlayerControllerHuman::_update(double dt, int x_limit, int y_limit, int ball_x, int ball_y)
{
	// do nothing
}

void PlayerControllerHuman::_handleEvent( Command& command ){

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
                    tiempo[(int)CommandType::KICK]=std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());;
					//std::cout<<time(NULL)<<std::endl;
                    //playerModel->kick(world.getBall().getModel());
                }

				log->debug("PlayerControllerHuman: apretando patear");
				break;
			}
			case CommandType::CHANGE_PLAYER: {
				if (this->getModel()->getHasControlOfTheBall() == false) {
					this->world.swap(this);
					log->debug("PlayerControllerHuman: cambiando el jugador controlado");
				}
				else {
					log->debug("PlayerControllerHuman: no se puede cambiar un jugador que tenga la pelota");
				}
				break;
			}
			case CommandType::PASS:{
                if(playerModel->getHasControlOfTheBall()) {
                    tiempo[(int)CommandType::PASS]=std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());;
                    //std::cout<<time(NULL)<<std::endl;
                    //playerModel->kick(world.getBall().getModel());
                }
				log->debug("PlayerControllerHuman: apretando pasar");
				break;
			}
            case CommandType::LONG_PASS:{
                if(playerModel->getHasControlOfTheBall()) {
                    tiempo[(int)CommandType::LONG_PASS]=std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());;
                    //std::cout<<time(NULL)<<std::endl;
                    //playerModel->kick(world.getBall().getModel());
                }
            break;
            };
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
        else if (command.type == CommandType::KICK) {
            //time_t actualTiempo = time(NULL);
			//std::cout<<time(NULL)<<std::endl;
			//std::cout<<tiempo[(int)CommandType::KICK]<<std::endl;
            if(playerModel->getHasControlOfTheBall()) {
                auto end = std::chrono::system_clock::now();
                double potencia= (end - tiempo[(int)CommandType::KICK]).count();
                playerModel->kickHighRegular(potencia,world.getBall().getModel());
            }
            log->debug("PlayerControllerHuman: soltando patear");
        }
        else if (command.type == CommandType::PASS) {
            if(playerModel->getHasControlOfTheBall()){
                auto end = std::chrono::system_clock::now();
                double potencia= (end - tiempo[(int)CommandType::PASS]).count();
                PlayerController* passController=world.getPlayerToPass(this);
                if(passController!=this){
                    playerModel->pass(passController->getModel(), world.getBall().getModel(), potencia);
                    this->world.swap(this);
                }else{
                    playerModel->kick(potencia,world.getBall().getModel());
                }
            }
            log->debug("PlayerControllerHuman: soltando pasar");
        }
        else if (command.type == CommandType::LONG_PASS) {
            auto end = std::chrono::system_clock::now();
            double potencia= (end - tiempo[(int)CommandType::LONG_PASS]).count();
            if(playerModel->getHasControlOfTheBall()){
                PlayerController* passController=world.getPlayerToPassLong(this);
                if(passController!=this){
                    playerModel->longPass(passController->getModel(), world.getBall().getModel(), potencia);
                    this->world.swap(this);
                }else{
                    playerModel->kickHigh(potencia,world.getBall().getModel());
                }
            }
            log->debug("PlayerControllerHuman: pasar largo");
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

User_ID PlayerControllerHuman::getUserId()
{
	return this->userId;
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
