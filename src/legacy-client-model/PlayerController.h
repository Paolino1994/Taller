//
// Created by federico on 11/04/18.
//

#pragma once

#include "PlayerModel.h"
#include "PlayerView.h"
#include "common/Log.h"

class PlayerController {

protected:

	PlayerModel * playerModel;
	PlayerView *playerView;

public:
	PlayerController(PlayerModel* model, PlayerView* view);
	virtual ~PlayerController() {};

	virtual Entity* getEntity();

	virtual PlayerModel* getModel();

	virtual PlayerView* getView();

	virtual void handleEvent(SDL_Event &e) = 0;

	virtual void update(double dt, int x_limit, int y_limit);

	virtual void swap(PlayerController* other);

	virtual bool isControllable() = 0; //isSwappable()

    bool hasControlOfTheBall();

    double getAngle();
};
