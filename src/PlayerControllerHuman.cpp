#include "PlayerControllerHuman.h"

PlayerControllerHuman::PlayerControllerHuman(PlayerModel * model, PlayerView * view):
	PlayerController(model, view),
	pressingUP(false),
	pressingDOWN(false),
	pressingLEFT(false),
	pressingRIGHT(false)
{
	this->log = Log::get_instance();
}


void PlayerControllerHuman::handleEvent( SDL_Event& e )
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
				log->debug("PlayerControllerHuman: apretando arriba");
                break;
            }
            case SDLK_DOWN: {
				this->pressingDOWN = true;
                //velY += MAX_VEL_Y;
                playerModel->changeVelY(MAX_VEL_Y);
				log->debug("PlayerControllerHuman: apretando abajo");
                break;
            }
            case SDLK_LEFT:{
				this->pressingLEFT = true;
                //velX -= MAX_VEL_X;
				playerModel->changeVelX(-MAX_VEL_X);
				log->debug("PlayerControllerHuman: apretando izquierda");
                break;
            }
            case SDLK_RIGHT:{
				this->pressingRIGHT = true;
                //velX += MAX_VEL_X;
				playerModel->changeVelX(MAX_VEL_X);

				log->debug("PlayerControllerHuman: apretando derecha");
                break;
            }
			case SDLK_SPACE: {
				playerModel->sweep();
				log->debug("PlayerControllerHuman: apretando barrer");
				break;
			}
			case SDLK_r: {
				playerModel->sprint();
				log->debug("PlayerControllerHuman: apretando correr");
				break;
			}
			case SDLK_RETURN: {
				playerModel->kick();
				log->debug("PlayerControllerHuman: apretando patear");
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

			log->debug("PlayerControllerHuman: soltando arriba");
		}
		else if (e.key.keysym.sym == SDLK_DOWN && this->pressingDOWN) {
			//velY -= MAX_VEL_Y;
			playerModel->changeVelY(-MAX_VEL_Y);
			log->debug("PlayerControllerHuman: soltando abajo");
		}
		else if (e.key.keysym.sym == SDLK_LEFT && this->pressingLEFT) {
			//velX += MAX_VEL_X;
			playerModel->changeVelX(MAX_VEL_X);
			log->debug("PlayerControllerHuman: soltando izquierda");
		}
		else if (e.key.keysym.sym == SDLK_RIGHT && this->pressingRIGHT) {
			//velX -= MAX_VEL_X;
			playerModel->changeVelX(-MAX_VEL_X);
			log->debug("PlayerControllerHuman: soltando derecha");
		}
		else if (e.key.keysym.sym == SDLK_r) {
			playerModel->stopSprinting();
			log->debug("PlayerControllerHuman: soltando correr");
		}

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


