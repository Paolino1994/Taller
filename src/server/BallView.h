//
// Created by federico on 30/04/18.
//

#ifndef UNTITLED1_BALLVIEW_H
#define UNTITLED1_BALLVIEW_H


#include "BallModel.h"
#include "Animation.h"


class BallView {
public:
    BallView(std::map<const std::string, Animation>& animMapperBall, BallModel& ballStill);

	//void render(int i, int i1, double angle);

    //void render(int screen_x,int screen_y);

    void update(double d);



private:
    std::vector<Animation> animation;

    BallModel& ballModel;

    BallState ballState;
};


#endif //UNTITLED1_BALLVIEW_H
