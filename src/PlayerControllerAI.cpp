
#include "PlayerControllerAI.h"
#include <cmath>

PlayerControllerAI::PlayerControllerAI(PlayerModel * model, PlayerView * view):
	PlayerController(model, view)
{
}

void PlayerControllerAI::update(double dt, int x_limit, int y_limit)
{
	// AI para volver a nuestra posicion inicial
	// Bien simple que vuelva a su posicion
	this->playerModel->stopSprinting(); //por las dudas

	int direction_x = this->playerModel->initial_x - this->playerModel->getX();
	int direction_y = this->playerModel->initial_y - this->playerModel->getY();

	if (abs(direction_x) > 5) {
		int sign_x = direction_x / abs(direction_x);
		this->playerModel->setVelX(sign_x * this->playerModel->getMaxVelX());
	}
	else {
		this->playerModel->setVelX(0);
	}

	if (abs(direction_y) > 5) {
		int sign_y = direction_y / abs(direction_y);
		this->playerModel->setVelY(sign_y * this->playerModel->getMaxVelY());
	}
	else {
		this->playerModel->setVelY(0);
	}

	PlayerController::update(dt, x_limit, y_limit);
}

