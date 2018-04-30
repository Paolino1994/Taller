//
// Created by federico on 30/04/18.
//

#include "BallController.h"

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
