//
// Created by federico on 30/04/18.
//
#pragma once

#include "Entity.h"
#include "BallModel.h"
#include "BallView.h"
#include "PlayerController.h"

class BallController {

private:
	BallModel ballModel;
	BallView ballView;

public:
	BallController(int initial_x, int initial_y, std::map<const std::string, Animation>& ballAnimMapper);

    BallModel& getModel();

    BallView& getView();

    void kick();

    void update(double dt, int x_limit, int y_limit, std::vector<PlayerController*>& playerControllers);

};