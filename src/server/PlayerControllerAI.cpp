#include "PlayerControllerAI.h"
#include <cmath>

PlayerControllerAI::PlayerControllerAI(PlayerModel * model, PlayerView * view):
	PlayerController(model, view)
{
	this->log = Log::get_instance();
	this->getModel()->setIsControlledByHuman(false);
}

PlayerControllerAI::PlayerControllerAI(PlayerController * other):
	PlayerController(other)
{
	this->log = Log::get_instance();
	this->getModel()->setIsControlledByHuman(false);
}

void PlayerControllerAI::_handleEvent(Command & c)
{
	//do nothing
}

User_ID PlayerControllerAI::getUserId()
{
	return AI_USER;
}

void PlayerControllerAI::_update(double dt, int x_limit, int y_limit, int ball_x, int ball_y)
{
	bool teamHasBall = this->playerModel->getTeam() == GameManager::get_instance().getTeamBallControl();
	FIELD_POSITION defendedGoal = GameManager::get_instance().getDefendedGoal(this->playerModel->getTeam());
	int positionModifierX = 0; // Esto es para que sea un poco mas fluido el movimiento y los jugadores no tengan un punto fijo de "centro" de area, si no que se adelantan o se atrasan segun tengan la pelota
	if (teamHasBall) {
		positionModifierX = defendedGoal == FIELD_POSITION::LEFT ? 100 : -100; // Si tiene la pelota y defiende en la izquiera entonces le tengo que sumar a la posicion (para que ataque). Si defiende a la derecha le tengo que restar.
	} else {
		positionModifierX = defendedGoal == FIELD_POSITION::LEFT ? -100 : 100; // Si NO tiene la pelota y defiende en la izquiera entonces le tengo que restar a la posicion (para que defienda). Si defiende a la derecha le tengo que sumar.
	}
	if(UserManager::get_instance().game_started()) { // FORMA CABEZA DE EVITAR QUE SE MUEVAN LOS JUGADORES ANTES DE QUE EMPIECE EL PARTIDO
		// AI para volver a nuestra posicion inicial
		// Bien simple que vuelva a su posicion
		this->playerModel->stopSprinting(); //por las dudas

		int max_distance_x = 0;
		int max_distance_y = 0;

		int margin = 30; // Aca controlamos los que se quedan locos

		if(teamHasBall) {
			max_distance_x = this->playerModel->getAtack_distance_x(); 
			max_distance_y = this->playerModel->getAtack_distance_y();
		} else {
			max_distance_x = this->playerModel->getDefence_distance_x(); 
			max_distance_y = this->playerModel->getDefence_distance_y();
		}


		int direction_x_initial = this->playerModel->getInitial_x() + positionModifierX - this->playerModel->getX();
		int direction_y_initial = this->playerModel->getInitial_y() - this->playerModel->getY();

		int ball_in_area_x = ball_x - this->playerModel->getInitial_x() + positionModifierX;
		int ball_in_area_y = ball_y - this->playerModel->getInitial_y();

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
		} else if (abs(direction_x_initial) > max_distance_x && (abs(direction_x_initial) > max_distance_x + margin)) { // Si el jugador esta afuera de su zona debe volver a la zona
			direction_x_goto = (direction_x_initial > 0) ? 1 : -1;
		} 
		else if(abs(direction_x_initial) >= max_distance_x && abs(direction_x_initial) < max_distance_x + margin){ // Si el jugador esta en limite de su zona entonces me quedo parado
			direction_x_goto = 0;
		}
		else {  // El jugador siempre intenta acercarse a la pelota en todo momento
			direction_x_goto = (direction_x_ball >= 0) ? 1 : -1;
		}
		if (teamHasBall && abs(direction_x_ball) < 100 ) {// Si se acerca mucho a la pelota y la tiene su equipo entonces se queda quieto
			direction_x_goto = 0;
		} 

		// Lo mismo pero en Y
		if (abs(ball_in_area_y) < max_distance_y) {
			if(abs(direction_y_ball) > 5){
					direction_y_goto = (direction_y_ball >= 0) ? 1 : -1;
			} else {
				direction_y_goto = 0;
			}
		} else if (abs(direction_y_initial) > max_distance_y && (abs(direction_y_initial) > max_distance_y + margin )) {
			direction_y_goto = (direction_y_ball >= 0) ? 1 : -1;
		} 
		else if(abs(direction_y_initial) >= max_distance_y && abs(direction_y_initial) < max_distance_y + 10){
			direction_y_goto = 0;
		}
		else {
			direction_y_goto = (direction_y_ball >= 0) ? 1 : -1;
		}
		if (teamHasBall && abs(direction_y_ball) < 100) { // Si se acerca mucho a la pelota y la tiene su equipo entonces se queda quieto
			direction_y_goto = 0;
		} 

		this->playerModel->setVelX(direction_x_goto * this->playerModel->getMaxVelX());
		this->returning_x = true;
		this->playerModel->setVelY(direction_y_goto * this->playerModel->getMaxVelY());
		this->returning_y = true;

		if(abs(direction_x_ball) < 5 && abs(direction_y_ball) < 5) {
			this->playerModel->sweep();
		}
	}
}

bool PlayerControllerAI::isControllable() {
	return true;
}

