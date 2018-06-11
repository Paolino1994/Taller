//
// Created by federico on 11/04/18.
//
#include <iostream>
#include "PlayerController.h"

#include "scripted/PlayerKickOffSetupState.h"
#include "scripted/PlayerGoalKickSetupState.h"
#include "server/GameManager.h"

void PlayerController::checkStateChange()
{
	if (this->scriptedState && this->scriptedState->isFinished()) {
		this->setScriptedState(this->scriptedState->getNextState());
	}
}

PlayerController::PlayerController(PlayerModel * model, PlayerView * view, PlayerControllerScriptedState * scriptedState):
	playerModel(model),
	playerView(view),
	scriptedState(scriptedState)
{
	this->registerTo(EventID::PERIOD_END);
	this->registerTo(EventID::GOAL);
	this->registerTo(EventID::GOAL_KICK);
	// IMPORTANTE: desregistrarme en el destructor porque es posible el matado de controllers cuando entran/salen jugadores!
}

PlayerController::~PlayerController() {
	this->unregisterFrom(EventID::PERIOD_END);
	this->unregisterFrom(EventID::GOAL);
	this->unregisterFrom(EventID::GOAL_KICK);
}

PlayerController::PlayerController(PlayerModel * model, PlayerView * view):
	PlayerController(model, view, nullptr)
{}

PlayerController::PlayerController(PlayerController * other):
	PlayerController(other->getModel(), other->getView(), other->scriptedState.release())
{}

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

void PlayerController::setScriptedState(PlayerControllerScriptedState * newState)
{
	this->scriptedState.reset(newState);
}

void PlayerController::handleEvent(Command & command)
{
	this->checkStateChange();
	if (this->scriptedState) {
		std::cout << "Scripted state handleando un command!" << std::endl;
		if (this->scriptedState->handleEvent(command)) {
			this->_handleEvent(command);
		}
	}
	else {
		this->_handleEvent(command);
	}
}

void PlayerController::update(double dt, int x_limit, int y_limit, int ball_x, int ball_y) {
	this->checkStateChange();
	if (this->scriptedState) {
		this->scriptedState->update(dt, x_limit, y_limit, ball_x, ball_y);
	}
	else {
		this->_update(dt, x_limit, y_limit, ball_x, ball_y);
	}
	this->playerModel->update(dt, x_limit, y_limit);
	this->playerView->update(dt);
}

void PlayerController::swap(PlayerController * otherController)
{
	Log* log = Log::get_instance();
	log->debug("PlayerController: cambiando jugador");

	bool isControlledHumanForOther = this->playerModel->getIsControlledByHuman();
	this->playerModel->setIsControlledByHuman(otherController->playerModel->getIsControlledByHuman());
	otherController->playerModel->setIsControlledByHuman(isControlledHumanForOther);

	PlayerModel* modelForOther = this->playerModel;
	this->playerModel = otherController->playerModel;
	otherController->playerModel = modelForOther;

	auto tempView = this->playerView;
	this->playerView = otherController->playerView;
	otherController->playerView = tempView;

	if(this->hasControlOfTheBall()){
		GameManager::get_instance().setLastBallControlUser(this->getUserId());
	}

	this->scriptedState.swap(otherController->scriptedState);
}

bool PlayerController::hasControlOfTheBall() {
	return this->playerModel->getHasControlOfTheBall();
}

double PlayerController::getAngle() {
    return playerModel->getAngle();
}

void PlayerController::handle(PeriodEndEvent & e)
{
	this->setScriptedState(new PlayerKickOffSetupState(*this->getModel(), e.teamToKickOffNextPeriod,
		GameManager::getKickOffSideAfterPeriodEnd()));
}

void PlayerController::handle(GoalEvent & e)
{
	// TODO: mandar a un estado de festejo
	// y, por ejemlo, ese estado aguantarlo un tiempo X y despues setearle este:
	this->setScriptedState(new PlayerKickOffSetupState(*this->getModel(), GameManager::getKickOffTeamAfterGoal(e),
		GameManager::get_instance().getKickOffSideAfterGoal(e)));
}

void PlayerController::handle(GoalKickEvent & e)
{
	this->setScriptedState(new PlayerGoalKickSetupState(*this->getModel(), e.team, e.goal));
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
