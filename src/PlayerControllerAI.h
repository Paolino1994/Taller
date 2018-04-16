#pragma once

#include "PlayerController.h"

class PlayerControllerAI : public PlayerController {

public:
	PlayerControllerAI(PlayerModel* model, PlayerView* view);

	virtual void update(double dt, int x_limit, int y_limit);

};

