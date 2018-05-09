//
// Created by federico on 30/04/18.
//

#include "BallView.h"
#include "BallController.h"

BallView::BallView(std::map<const std::string, Animation> animMapperBall, BallModel *ballModel) {
    //animation=animMapperBall;
    model=ballModel;
    priorState=QUIESCENT;
    for(auto elem : animMapperBall)
        animation.push_back(elem.second);
}

void BallView::render(int i, int i1,double angle) {
    int xCorrection=BallController::getInstance()->getModel()->getXCorrection();
    int yCorrection=BallController::getInstance()->getModel()->getYCorrection();

    BallState newState = BallController::getInstance()->getModel()->getState();
    if (priorState != newState) {
        animation[newState].reset();
        priorState = newState;
    }
    animation[priorState].render(i+xCorrection,i1+yCorrection,angle);
}


void BallView::update(double deltaTime) {
    animation[priorState].update(deltaTime);
}

void BallView::render(int screen_x, int screen_y) {
    BallState newState = BallController::getInstance()->getModel()->getState();
    if (priorState != newState) {
        animation[newState].reset();
        priorState = newState;
    }
    int angle=BallController::getInstance()->getModel()->getAngle();
    animation[priorState].render(screen_x,screen_y,angle);

}





