//
// Created by federico on 30/04/18.
//

#ifndef UNTITLED1_BALL_H
#define UNTITLED1_BALL_H


#include "Entity.h"

class BallModel: public Entity {

public:
    BallModel(double kickOff_x, double kickOff_y, int X, int Y);

    BallModel(double kickOff_x, double kickOff_y);

    int getWidth();
    int getHeight();

private:
    int posY;
    int posX;
};


#endif //UNTITLED1_BALL_H
