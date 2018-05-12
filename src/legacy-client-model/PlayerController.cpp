//
// Created by federico on 11/04/18.
//

#include "PlayerController.h"

PlayerController::PlayerController(PlayerModel * model, PlayerView * view):
	playerModel(model),
	playerView(view)
{
	playerModel->setIsControlledByHuman(false);
}

Entity * PlayerController::getEntity()
{
	return getModel();
}

PlayerModel * PlayerController::getModel()
{
	return this->playerModel;
}

PlayerView * PlayerController::getView()
{
	return this->playerView;
}

void PlayerController::update(double dt, int x_limit, int y_limit)
{
	this->playerModel->update(dt, x_limit, y_limit);
	this->playerView->update(dt);
}

void PlayerController::swap(PlayerController * otherController)
{
	Log* log = Log::get_instance();
	log->debug("PlayerController: cambiando jugador");

	auto temp = this->playerModel;
	this->playerModel->setIsControlledByHuman(false);
	this->playerModel = otherController->playerModel;
	this->playerModel->setIsControlledByHuman(true);
	otherController->playerModel = temp;

	auto tempView = this->playerView;
	this->playerView = otherController->playerView;
	otherController->playerView = tempView;
}

bool PlayerController::hasControlOfTheBall() {
	return this->playerModel->getHasControlOfTheBall();
}

double PlayerController::getAngle() {
    return playerModel->getAngle();
}
