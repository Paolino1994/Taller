//
// Created by federico on 30/04/18.
//

#include <iostream>
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
    calculateCollision(playerControllers);
    ballView.update(dt);
}

void BallController::calculateCollision(std::vector<PlayerController *> &playerControllers) {
    int x=ballModel.getX();
    int y=ballModel.getY();
    int i=0;
    for (PlayerController* controller : playerControllers) {
        //AGREGAR UN QUITAR BALLCONTROL A TODOS LOS DEMAS   takeBallControll(playerControllers,i);
            int xPlayer=controller->getModel()->getCenterX();
            int yPlayer=controller->getModel()->getCenterY();
            //std::cout<<"X: "<<std::to_string(x)<<" X Player: "<<std::to_string(xPlayer)<<std::endl;
            //std::cout<<"Y: "<<std::to_string(y)<<" Y Player: "<<std::to_string(yPlayer)<<std::endl;
            if(abs(x-xPlayer)<20 && abs(y-yPlayer)<20){
                controller->getModel()->setHasControlOfTheBall(true);
                /*for (int j=0;j<(int)playerControllers.size();j++) {
                    if(playerControllers[j]->getModel()->getTeam()!=controller->getModel()->getTeam()){
                        controller->getModel()->setHasControlOfTheBall(false);
                    }
                }*/

            }

        i++;
    }

}



