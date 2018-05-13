#pragma once

#include "World.h"
#include "PlayerController.h"
#include "common/Log.h"

class PlayerControllerHuman : public PlayerController {

public:
	PlayerControllerHuman(PlayerModel* model, PlayerView* view, World& world, User_ID userId);

	virtual void handleEvent(Command &e);

	//virtual void update(double dt, int x_limit, int y_limit);

	virtual void swap(PlayerController* otherController);

	virtual bool isControllable();

	virtual User_ID getUserId();

private:
	World & world;
	User_ID userId;
	bool pressingUP;
	bool pressingDOWN;
	bool pressingLEFT;
	bool pressingRIGHT;
	Log* log;
};
