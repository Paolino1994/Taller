//
// Created by federico on 11/04/18.
//

#pragma once

#include "PlayerModel.h"
#include "PlayerView.h"
#include "Log.h"

class PlayerController {

protected:

	PlayerModel * playerModel;
	PlayerView *playerView;

public:
	PlayerController(PlayerModel* model, PlayerView* view);
	virtual ~PlayerController() {};

	virtual Entity* getEntity();

	virtual PlayerView* getView();

	virtual void update(double dt, int x_limit, int y_limit);

	virtual void swap(PlayerController* other);

};
