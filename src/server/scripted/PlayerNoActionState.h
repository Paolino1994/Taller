#pragma once
#include "PlayerControllerScriptedState.h"
#include "common/EventHandler.h"

class PlayerNoActionState: public PlayerControllerScriptedState, public EventHandler
{
private:
	PlayerModel & player;
	Team teamToKickOff;
public:
	PlayerNoActionState(PlayerModel& player);
	virtual ~PlayerNoActionState();

	virtual bool handleEvent(Command &e);

	virtual void update(double dt, int x_limit, int y_limit, int ball_x, int ball_y);

	virtual PlayerControllerScriptedState* getNextState();

	// Events to handle
	virtual void handle(GameRestartEvent& e);
};

