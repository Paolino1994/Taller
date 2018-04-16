#pragma once

#include "PlayerController.h"

class PlayerControllerHuman : public PlayerController {

public:
	PlayerControllerHuman(PlayerModel* model, PlayerView* view);

	void handleEvent(SDL_Event &e);

	//virtual void update(double dt, int x_limit, int y_limit);

	virtual void swap(PlayerController* otherController);

private:
	bool pressingUP;
	bool pressingDOWN;
	bool pressingLEFT;
	bool pressingRIGHT;
};
