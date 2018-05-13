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

void PlayerController::serialize(player_view_data_t& player_view_data) {
	player_view_data.playerId = this->playerModel->getPlayerId();
	player_view_data.userId = this->getUserId();
	player_view_data.x = this->getModel()->getX();
	player_view_data.y = this->getModel()->getY();
	player_view_data.angle = this->getModel()->getAngle();
	player_view_data.state = this->getModel()->getState();
	player_view_data.team = this->getModel()->getTeam();
	this->getView()->serialize(player_view_data);
}