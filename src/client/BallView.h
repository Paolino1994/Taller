//
// Created by federico on 30/04/18.
//

#ifndef UNTITLED1_BALLVIEW_H
#define UNTITLED1_BALLVIEW_H


#include "BallModel.h"
#include "Animation.h"


class BallView {
public:
    BallView(std::map<const std::string, Animation> animMapperBall, BallModel *ballStill);

    void render(int i, int i1, double angle);

    void render();

    void update(double d);

private:
    std::vector<Animation> animation;

    BallModel* model;

    BallState priorState;
};


#endif //UNTITLED1_BALLVIEW_H
