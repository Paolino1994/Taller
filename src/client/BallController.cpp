//
// Created by federico on 30/04/18.
//

#include "BallController.h"

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

void BallController::kick() {
    ballModel.kick();
}

void BallController::update(double dt, int x_limit, int y_limit, std::vector<PlayerController*>& playerControllers){
    ballModel.update(dt,x_limit,y_limit, playerControllers);
    ballView.update(dt);
}



