#pragma once

#include "PlayerControllerScriptedState.h"
#include "common/EventHandler.h"
#include "server/PlayerModel.h"
#include "common/GameConstants.h"

class PlayerGoalKickSetupState: public PlayerControllerScriptedState, public EventHandler
{
private:
	PlayerModel & player;
	Team goalKickTeam;
	int goal;
	double secondsPassed;

	void goTo(int x, int y, int delta);
	static const int goalKick_delta = 10;
	static const int secondsToWaitForAutomaticGoalKick = 5;
public:
	PlayerGoalKickSetupState(PlayerModel& player, Team goalKickTeam, int goal);
	virtual ~PlayerGoalKickSetupState();

	virtual void handleEvent(Command &e);

	//virtual void update(double dt, int x_limit, int y_limit);

	virtual void update(double dt, int x_limit, int y_limit, int ball_x, int ball_y);

	virtual void handle(KickEvent& kickEvent);

	virtual PlayerControllerScriptedState* getNextState();
};
