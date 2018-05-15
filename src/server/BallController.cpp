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

void BallController::kick() {
    ballModel.kick();
}

void BallController::update(double dt, int x_limit, int y_limit, std::vector<PlayerController*>& playerControllers){
    ballModel.update(dt,x_limit,y_limit, playerControllers);
    calculateCollision(playerControllers);
    ballView.update(dt);
}

void BallController::calculateCollision(std::vector<PlayerController *> &playerControllers) {
	int x = ballModel.getX();
	int y = ballModel.getY();
	int i = 0;
	for (PlayerController* controller : playerControllers) {
		if(controller->getModel()->isColisionable){
			int xPlayer = controller->getModel()->getCenterX();
			int yPlayer = controller->getModel()->getCenterY();
			if (abs(x - xPlayer)<20 && abs(y - yPlayer)<20) {
				if (!controller->getModel()->getHasControlOfTheBall()) {
					Log::get_instance()->debug("Colision, un nuevo jugador toma la pelota");
					controller->getModel()->setHasControlOfTheBall(true);
					ballModel.setVelX(controller->getModel()->getVelX());
					ballModel.setVelY(controller->getModel()->getVelY());
					//controller->getModel()->setAngle(-90+ballModel.getAngle());
					changeController(i, playerControllers);
				}
			}
		}

		i++;
	}
}

void BallController::changeController(int newController, std::vector<PlayerController *> &playerControllers) {
	int counter = 0;
	for (PlayerController* controller : playerControllers) {
		if (newController == counter) {
			controller->getModel()->setHasControlOfTheBall(true);
			//std::cout<<"Agarro la pelota"<< "Ball VelX: "<<ballModel.getVelX()<<" Ball VelY: "<<ballModel.getVelY()<<std::endl;
		}
		else {
			controller->getModel()->setHasControlOfTheBall(false);
		}
		counter++;
	}

}

void BallController::serialize(ball_view_data_t& ball_view_data) {
	ball_view_data.x = this->ballModel.getX();
	ball_view_data.y = this->ballModel.getY();
	ball_view_data.angle = this->ballModel.getAngle();
	ball_view_data.state = this->ballModel.getState();
	this->getView().serialize(ball_view_data);
}


