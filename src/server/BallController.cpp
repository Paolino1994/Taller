//
// Created by federico on 30/04/18.
//

#include <iostream>
#include "BallController.h"
#include "common/Log.h"

BallController::BallController(int initial_x, int initial_y, std::map<const std::string, Animation>& ballAnimMapper):
	ballModel(BallModel(initial_x, initial_y)),
	ballView(BallView(ballAnimMapper, ballModel))
{
}

BallModel& BallController::getModel() {
    return ballModel;
}

BallView& BallController::getView() {
    return ballView;
}

void BallController::update(double dt, int x_limit, int y_limit){
    ballModel.update(dt,x_limit,y_limit);
    ballView.update(dt);
}

void BallController::serialize(ball_view_data_t& ball_view_data) {
	ball_view_data.x = this->ballModel.getX();
	ball_view_data.y = this->ballModel.getY();
	ball_view_data.z = this->ballModel.getZ();
	ball_view_data.angle = this->ballModel.getAngle();
	ball_view_data.state = this->ballModel.getState();
	this->getView().serialize(ball_view_data);
}


