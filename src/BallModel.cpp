//
// Created by federico on 30/04/18.
//

#include <iostream>
#include "BallModel.h"
#include "GameConstants.h"
#define PI 3.14159265


BallModel::BallModel(double kickOff_x, double kickOff_y, int X, int Y) : Entity(kickOff_x, kickOff_y) {
    x=X;
    y=Y;
    state=QUIESCENT;

}

BallModel::BallModel(double kickOff_x, double kickOff_y) : Entity(kickOff_x, kickOff_y) {
    x=0;
    y=0;
    state=QUIESCENT;
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


void BallModel::correctPosition() {
    int xCor = getXCorrection();
    int yCor = getYCorrection();
    //std::cout<<"X:"<<x<<" Correccion:"<<xCor<<std::endl;
    x=x+xCor;

    y=y+yCor;

}

int BallModel::getXCorrection() {
    double initialcorrection=23.0;
    int angleToUse=angle;
    if(angleToUse>=90){
        angleToUse-=90;
    }else{
        angleToUse+=270;
    }
    double multiplier=cos(angleToUse*PI/180.0);
    //std::cout<<"Angle:"<<angleToUse<<" Multiplicador:  "<<multiplier<<std::endl;

    double correction=multiplier*25;
    return initialcorrection+correction;
}

int BallModel::getYCorrection() {
    double initialcorrection=10.0;
    int angleToUse=angle;
    if(angleToUse>=90){
        angleToUse-=90;
    }else{
        angleToUse+=270;
    }
    double multiplier=sin(angleToUse*PI/180.0);
    //std::cout<<"Angle:"<<angleToUse<<" Multiplicador:  "<<multiplier<<std::endl;

    double correction=multiplier*25;
    return initialcorrection+correction;

}

void BallModel::setAngle(double d) {
    angle=d;
    correctPosition();

}

BallState BallModel::getState() {
    return state;
}

void BallModel::setState(BallState aState) {
    state=aState;

}



