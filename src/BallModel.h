//
// Created by federico on 30/04/18.
//

#ifndef UNTITLED1_BALL_H
#define UNTITLED1_BALL_H


#include "Entity.h"
#include <math.h>

class BallModel: public Entity {

public:
    BallModel(double kickOff_x, double kickOff_y, int X, int Y);

    BallModel(double kickOff_x, double kickOff_y);

    int getWidth();
    int getHeight();

    void setX(double d);

    void setY(double d);

    void correctPosition();

    int getXCorrection();

    int getYCorrection();

    void setAngle(double d);

private:
    double angle=0;
};


#endif //UNTITLED1_BALL_H
