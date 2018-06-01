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

		if (this->playerModel->getTeam() == GameManager::get_instance()->getTeamBallControl()) {
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

		int direction_x_goto = 0;
		int direction_y_goto = 0;


		if (abs(ball_in_area_x) < max_distance_x) { // Si la pelota esta en la zona entonces la va a buscar
			if(abs(direction_x_ball) > 5){ // Si esta muy cerca de x deja de correr porque si no queda girando en el lugar y es feo
				direction_x_goto = (direction_x_ball >= 0) ? 1 : -1;
			} else {
				direction_x_goto = 0;
			}
		} else if (abs(direction_x_initial) > max_distance_x && (abs(direction_x_initial) > max_distance_x + 10)) { // Si el jugador esta afuera de su zona debe volver a la zona
			direction_x_goto = (direction_x_initial > 0) ? 1 : -1;
		} 
		else if(abs(direction_x_initial) >= max_distance_x && abs(direction_x_initial) < max_distance_x + 10){ // Si el jugador esta en limite de su zona entonces me quedo parado
			direction_x_goto = 0;
		}
		else {  // El jugador siempre intenta acercarse a la pelota en todo momento
			direction_x_goto = (direction_x_ball >= 0) ? 1 : -1;
		}

		// Lo mismo pero en Y
		if (abs(ball_in_area_y) < max_distance_y) {
			if(abs(direction_y_ball) > 5){
				direction_y_goto = (direction_y_ball >= 0) ? 1 : -1;
			} else {
				direction_y_goto = 0;
			}
		} else if (abs(direction_y_initial) > max_distance_y && (abs(direction_y_initial) > max_distance_y + 10 )) {
			direction_y_goto = (direction_y_ball >= 0) ? 1 : -1;
		} 
		else if(abs(direction_y_initial) >= max_distance_y && abs(direction_y_initial) < max_distance_y + 10){
			direction_y_goto = 0;
		}
		else {
			direction_y_goto = (direction_y_ball >= 0) ? 1 : -1;
		}

		this->playerModel->setVelX(direction_x_goto * this->playerModel->getMaxVelX());
		this->returning_x = true;
		this->playerModel->setVelY(direction_y_goto * this->playerModel->getMaxVelY());
		this->returning_y = true;

		if(abs(direction_x_ball) < 5 && abs(direction_y_ball) < 5) {
			this->playerModel->sweep();
		}

		PlayerController::update(dt, x_limit, y_limit);
	}
}

bool PlayerControllerAI::isControllable() {
	return true;
}

