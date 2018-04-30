//
// Created by federico on 30/04/18.
//

#include "BallModel.h"
#include "GameConstants.h"



BallModel::BallModel(double kickOff_x, double kickOff_y, int X, int Y) : Entity(kickOff_x, kickOff_y) {
    x=X;
    y=Y;

}

BallModel::BallModel(double kickOff_x, double kickOff_y) : Entity(kickOff_x, kickOff_y) {
    x=0;
    y=0;
}

int BallModel::getWidth() {
    return 20;
}

int BallModel::getHeight() {
    return 20;
}

void BallModel::setX(double d) {
    x=d;
}

void BallModel::setY(double d) {
    y=d;

}

void BallModel::correctPosition(double angle) {
    int xCor = getXCorrection(angle-90);
    //int yCor = getYCorrection(angle);
    x=x+xCor;
    //y=y+yCor;

}

int BallModel::getXCorrection(double angle) {
    if (angle<=90){
        return -5;
    }else if(angle>=270){
        return 5;
    }else{
        return 0;
    }
}

int BallModel::getYCorrection(double angle) {
    int initialcorrection=40;
    if (angle>90 && angle<270){
        initialcorrection+= 5;
    }else{
        if(angle!=90 || angle!=270){
            initialcorrection+= -5;        }

    }
    return initialcorrection;
}


