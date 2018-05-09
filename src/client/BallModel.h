//
// Created by federico on 30/04/18.
//

#ifndef UNTITLED1_BALL_H
#define UNTITLED1_BALL_H


#include "Entity.h"
#include "GameConstants.h"
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

    BallState getState();

    void setState(BallState state);

    void kick();

    void update(double d, int i, int i1);

    int getAngle();

private:
    double angle=0;

    BallState state;

    void setVelX(double d);

    void setVelY(double vel);

    double velX;

    double velY;

    double angleToUse;
};


#endif //UNTITLED1_BALL_H
