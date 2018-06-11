#include "PlayerNoActionState.h"

#include "PlayerKickOffSetupState.h"
#include "server/PlayerModel.h"
#include "server/GameManager.h"

PlayerNoActionState::PlayerNoActionState(PlayerModel & player) :
	PlayerControllerScriptedState(),
	player(player),
	teamToKickOff(Team::HOME)
{
	this->registerTo(EventID::GAME_RESTART);
	// Por las dudas nulleo velocidades
	player.stopSprinting();
	player.setVelX(0);
	player.setVelY(0);
}

PlayerNoActionState::~PlayerNoActionState()
{
	this->unregisterFrom(EventID::GAME_RESTART);
}

bool PlayerNoActionState::handleEvent(Command & e)
{
	// Do nothing
	return false;
}

void PlayerNoActionState::update(double dt, int x_limit, int y_limit, int ball_x, int ball_y)
{
	// Do nothing
	// Se supone que el nulleado en el constructor ya es suficiente
}

PlayerControllerScriptedState * PlayerNoActionState::getNextState()
{
	return new PlayerKickOffSetupState(this->player, this->teamToKickOff, GameManager::getKickOffSideAfterPeriodEnd());
}

void PlayerNoActionState::handle(GameRestartEvent & e)
{
	this->teamToKickOff = e.teamToKickOff;
	this->finished = true;
}
