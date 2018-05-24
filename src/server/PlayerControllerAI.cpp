#include "PlayerControllerAI.h"
#include <cmath>

PlayerControllerAI::PlayerControllerAI(PlayerModel * model, PlayerView * view):
	PlayerController(model, view)
{
	this->log = Log::get_instance();
}

void PlayerControllerAI::handleEvent(Command & e)
{
	//do nothing
}

User_ID PlayerControllerAI::getUserId()
{
	return AI_USER;
}

void PlayerControllerAI::update(double dt, int x_limit, int y_limit, int ball_x, int ball_y)
{
	if(UserManager::get_instance().game_started()) { // FORMA CABEZA DE EVITAR QUE SE MUEVAN LOS JUGADORES ANTES DE QUE EMPIECE EL PARTIDO
		// AI para volver a nuestra posicion inicial
		// Bien simple que vuelva a su posicion
		this->playerModel->stopSprinting(); //por las dudas

		// Si el jugador tiene la pelota entonces no hago nada (por ahora)
		if(this->playerModel->getHasControlOfTheBall()){
			this->playerModel->setVelX(0);
			this->playerModel->setVelY(0);

			PlayerController::update(dt, x_limit, y_limit);
			return;
		}

		int max_distance_x = this->playerModel->getDistance_x();
		int max_distance_y = this->playerModel->getDistance_y();

		int direction_x_initial = this->playerModel->initial_x - this->playerModel->getX();
		int direction_y_initial = this->playerModel->initial_y - this->playerModel->getY();

		int ball_in_area_x = ball_x - this->playerModel->initial_x;
		int ball_in_area_y = ball_y - this->playerModel->initial_y;

		int direction_x_ball = ball_x - this->playerModel->getX();
		int direction_y_ball = ball_y - this->playerModel->getY();


		if (abs(ball_in_area_x) < max_distance_x) { // Si la pelota esta en la zona entonces la va a buscar
			if(abs(direction_x_ball) > 5){ // Si esta muy cerca de x deja de correr porque si no queda girando en el lugar y es feo
				int sign_x = (direction_x_ball >= 0) ? 1 : -1;
				this->playerModel->setVelX(sign_x * this->playerModel->getMaxVelX());
				this->returning_x = true;
			} else {
				this->playerModel->setVelX(0);
			}
		} else if (abs(direction_x_initial) > max_distance_x && (abs(direction_x_initial) > max_distance_x + 10)) { // Si el jugador esta afuera de su zona debe volver a la zona
			int sign_x = (direction_x_initial > 0) ? 1 : -1;
			this->playerModel->setVelX(sign_x * this->playerModel->getMaxVelX());
			this->returning_x = true;
		} 
		else if(abs(direction_x_initial) >= max_distance_x && abs(direction_x_initial) < max_distance_x + 10){ // Si el jugador esta en limite de su zona entonces me quedo parado
			this->playerModel->setVelX(0);
		}
		else {  // El jugador siempre intenta acercarse a la pelota en todo momento
			int sign_x = (direction_x_ball >= 0) ? 1 : -1;
			this->playerModel->setVelX(sign_x * this->playerModel->getMaxVelX());
			this->returning_x = true;
		}

		// Lo mismo pero en Y
		if (abs(ball_in_area_y) < max_distance_y) {
			if(abs(direction_y_ball) > 5){
				int sign_y = (direction_y_ball >= 0) ? 1 : -1;
				this->playerModel->setVelY(sign_y * this->playerModel->getMaxVelY());
				this->returning_y = true;
			} else {
				this->playerModel->setVelY(0);
			}
		} else if (abs(direction_y_initial) > 100 && (abs(direction_y_initial) > 110)) {
			int sign_y = (direction_y_initial > 0) ? 1 : -1;
			this->playerModel->setVelY(sign_y * this->playerModel->getMaxVelY());
			this->returning_y = true;
		} 
		else if(abs(direction_y_initial) >= 100 && abs(direction_y_initial) < 110){
			this->playerModel->setVelY(0);
		}
		else {
			int sign_y = (direction_y_ball >= 0) ? 1 : -1;
			this->playerModel->setVelY(sign_y * this->playerModel->getMaxVelY());
			this->returning_y = true;
		}

		PlayerController::update(dt, x_limit, y_limit);
	}
}

bool PlayerControllerAI::isControllable() {
	return true;
}

