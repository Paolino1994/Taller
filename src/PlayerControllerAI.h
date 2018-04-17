#pragma once

#include "PlayerController.h"
#include "Log.h"

class PlayerControllerAI : public PlayerController {

private:
	Log* log;
	bool returning_x = false;
	bool returning_y = false;

public:
	PlayerControllerAI(PlayerModel* model, PlayerView* view);

	virtual void update(double dt, int x_limit, int y_limit);

};

