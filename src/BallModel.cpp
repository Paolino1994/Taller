//
// Created by federico on 30/04/18.
//

#include "BallModel.h"
#include "GameConstants.h"

BallModel::BallModel(double kickOff_x, double kickOff_y, int X, int Y) : Entity(kickOff_x, kickOff_y) {
    posX=X;
    posY=Y;

}

BallModel::BallModel(double kickOff_x, double kickOff_y) : Entity(kickOff_x, kickOff_y) {
    posX=0;
    posY=0;
}

int BallModel::getWidth() {
    return 20;
}

int BallModel::getHeight() {
    return 20;
}

