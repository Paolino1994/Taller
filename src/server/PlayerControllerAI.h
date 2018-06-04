#pragma once

#include "PlayerController.h"
#include "UserManager.h"
#include "GameManager.h"
#include "common/Log.h"

class PlayerControllerAI : public PlayerController {

private:
	Log* log;
	bool returning_x = false;
	bool returning_y = false;

public:
	PlayerControllerAI(PlayerModel* model, PlayerView* view);
	PlayerControllerAI(PlayerController* other);

	virtual void _handleEvent(Command &e);

	virtual void _update(double dt, int x_limit, int y_limit, int ball_x, int ball_y);

	virtual bool isControllable();

	virtual User_ID getUserId();

};

