#pragma once

#include "PlayerController.h"
#include "common/Log.h"

class PlayerControllerAI : public PlayerController {

private:
	Log* log;
	bool returning_x = false;
	bool returning_y = false;

public:
	PlayerControllerAI(PlayerModel* model, PlayerView* view);

	virtual void handleEvent(Command &e);

	virtual void update(double dt, int x_limit, int y_limit);

	virtual bool isControllable();

};

