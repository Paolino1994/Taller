//
// Created by federico on 30/04/18.
//

#include "BallView.h"

BallView::BallView(std::map<const std::string, Animation> animMapperBall, sprite_info ballStill, BallModel* ballModel){
    //animation=animMapperBall;
    model=ballModel;
    sprite=ballStill;
    for(auto elem : animMapperBall)
        animation.push_back(elem.second);
}

void BallView::render(int i, int i1, double d) {
    animation[0].render(i,i1,d);
}




