//
// Created by federico on 11/04/18.
//

#pragma once

#include "PlayerModel.h"
#include "PlayerView.h"
#include "common/Log.h"
#include "common/Request.h" //pasar a GameConstants.h?

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

	virtual void handleEvent(Command &e) = 0;

	virtual void update(double dt, int x_limit, int y_limit);

	virtual void update(double dt, int x_limit, int y_limit, int ball_x, int ball_y);

	virtual void swap(PlayerController* other);

	virtual bool isControllable() = 0; //isSwappable()

	virtual User_ID getUserId() = 0;
	
	virtual void serialize(player_view_data_t& player_view_data);

    bool hasControlOfTheBall();

    double getAngle();
};
