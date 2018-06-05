#include "PlayerGoalKickSetupState.h"

#include <iostream>
#include <cmath>

#include "common/EventQueue.h"
#include "common/Request.h"


void PlayerGoalKickSetupState::goTo(int x, int y, int delta)
{
	// Que vayan al (x,y) con margenes delta
	this->player.stopSprinting(); //por las dudas

	int direction_x = x - this->player.getCenterX();
	int direction_y = y - this->player.getCenterY();

	if (abs(direction_x) > delta) {
		int sign_x = direction_x / abs(direction_x);
		this->player.setVelX(sign_x * this->player.getMaxVelX());
	}
	else {
		this->player.setVelX(0);
	}

	if (abs(direction_y) > delta) {
		int sign_y = direction_y / abs(direction_y);
		this->player.setVelY(sign_y * this->player.getMaxVelY());
	}
	else {
		this->player.setVelY(0);
	}
}

PlayerGoalKickSetupState::PlayerGoalKickSetupState(PlayerModel& player, Team goalKickTeam, int goal):
	player(player),
	goalKickTeam(goalKickTeam),
	goal(goal),
	secondsPassed(0)
{
	this->registerTo(EventID::KICK); // TODO ver: probablemente nuevo evento: EventID::GOAL_KICK
}


PlayerGoalKickSetupState::~PlayerGoalKickSetupState()
{
	// OJO: quizas le cambian el estado al controller mientras trabajo
	this->unregisterFrom(EventID::KICK);
}

void PlayerGoalKickSetupState::handleEvent(Command & command)
{
	if (this->player.isGoalKeeper() && this->player.getHasControlOfTheBall()
		&& ((this->goal == FIELD_POSITION::LEFT && this->player.isNearThisPoint(YAML::LEFT_GOAL_KICK_POINT_X, YAML::FIELD_CENTER_Y, goalKick_delta)) || (this->goal == FIELD_POSITION::RIGHT && this->player.isNearThisPoint(YAML::RIGHT_GOAL_KICK_POINT_X, YAML::FIELD_CENTER_Y, goalKick_delta)))) {
		// TODO: permitir setearle la dirección para sacar del medio
		// hay que jugar mucho con posicion del jugador relativa a la pelota
		// por ahora solo dejamos que saque!
		if (command.key == CommandKey::KEY_DOWN && command.type == CommandType::PASS) {
			// TODO: falta hacer saque real!
			std::cout << "Sacando la bocha!" << std::endl;
			EventQueue::get().push(std::make_shared<KickEvent>(this->player));
		}
	}
	// else -> no podes hacer nada!
}

void PlayerGoalKickSetupState::update(double dt, int x_limit, int y_limit, int ball_x, int ball_y)
{
	// TODO ver: tratar antes, de cambiar el controller a uno controlado por humano
	//  -> enchufar en un sistema(!?!?): Sistema que siempre trata de swapear un human controller al chabon que tenga la pelota (ver bien)

	if (this->goalKickTeam == this->player.getTeam() && this->player.isGoalKeeper()) {
		if (this->player.getHasControlOfTheBall()) {
			// llevar al mediocampo!
			//std::cout << "Llendo al mediocampo" << std::endl;
			if (this->goal == FIELD_POSITION::LEFT) {
				goTo(YAML::LEFT_GOAL_KICK_POINT_X, YAML::FIELD_CENTER_Y, 10);
			} else {
				goTo(YAML::RIGHT_GOAL_KICK_POINT_X, YAML::FIELD_CENTER_Y, 10);
			}

			// Sacar solo despues de un tiempito si no estoy controlado por un humano
			if ((this->goal == FIELD_POSITION::LEFT && this->player.isNearThisPoint(YAML::LEFT_GOAL_KICK_POINT_X, YAML::FIELD_CENTER_Y, goalKick_delta)) || (this->goal == FIELD_POSITION::RIGHT && this->player.isNearThisPoint(YAML::RIGHT_GOAL_KICK_POINT_X, YAML::FIELD_CENTER_Y, goalKick_delta))) {
				//TODO if ya esta en el centro, mirar a mi lado!
				this->secondsPassed += dt;
				if (!this->player.getIsControlledByHuman() && this->secondsPassed >= secondsToWaitForAutomaticGoalKick) {
					// TODO: falta hacer saque real!
					std::cout << "Sacando la bocha de forma automatica!" << std::endl;
					EventQueue::get().push(std::make_shared<KickEvent>(this->player));
				}
			}
		}
		else {
			// buscar la pelota
			goTo(ball_x, ball_y, 0);
		}
	}
	else {
		// tengo que ir a la posicion de mi formacion
		this->player.setHasControlOfTheBall(false);
		goTo(player.getKickOff_x(), player.getKickOff_y(), 10);
	}


}

void PlayerGoalKickSetupState::handle(KickEvent & kickEvent)
{
	this->finished = true;
	this->unregisterFrom(EventID::KICK);
}

PlayerControllerScriptedState * PlayerGoalKickSetupState::getNextState()
{
	return nullptr;
}
