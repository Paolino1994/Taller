//
// Created by federico on 30/04/18.
//

#include "BallController.h"

BallController *instance=NULL;

void BallController::initialize(BallModel* model, BallView* view) {
    if (instance == NULL) {
        instance = new BallController(model, view);
    }
}
BallController* BallController::getInstance(){
    return instance;
}



BallModel* BallController::getModel() {
    return ballModel;
}

BallController::BallController(BallModel* model, BallView* view){
    ballModel=model;
    ballView=view;

}

BallView* BallController::getView() {
    return ballView;
}
