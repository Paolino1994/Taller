#pragma once

#include "PlayerControllerScriptedState.h"
#include "common/EventHandler.h"
#include "server/PlayerModel.h"

class PlayerKickOffSetupState: public PlayerControllerScriptedState, public EventHandler
{
private:
	PlayerModel & player;
	Team kickOffTeam;
	double secondsPassed;

	void goTo(int x, int y, int delta);
	static const int kickOff_delta = 10;
	static const int secondsToWaitForAutomaticKickOff = 5;
public:
	PlayerKickOffSetupState(PlayerModel& player, Team kickOffTeam);
	virtual ~PlayerKickOffSetupState();

	virtual void handleEvent(Command &e);

	//virtual void update(double dt, int x_limit, int y_limit);

	virtual void update(double dt, int x_limit, int y_limit, int ball_x, int ball_y);

	virtual void handle(KickEvent& kickEvent);

	virtual PlayerControllerScriptedState* getNextState();
};

