//
// Created by federico on 30/04/18.
//

#include "BallView.h"
#include "BallController.h"

BallView::BallView(std::map<const std::string, Animation> animMapperBall, sprite_info ballStill, BallModel* ballModel){
    //animation=animMapperBall;
    model=ballModel;
    sprite=ballStill;
    for(auto elem : animMapperBall)
        animation.push_back(elem.second);
}

void BallView::render(int i, int i1,double angle) {
    int xCorrection=BallController::getInstance()->getModel()->getXCorrection(angle);
    int yCorrection=BallController::getInstance()->getModel()->getYCorrection(angle);
    animation[0].render(i+xCorrection,i1+yCorrection,angle);
}

void BallView::render() {
    animation[0].render(model->getX(),model->getY(),0.0);
}

void BallView::update(double deltaTime) {
    animation[0].update(deltaTime);
}




