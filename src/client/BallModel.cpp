//
// Created by federico on 30/04/18.
//

#include <iostream>
#include "BallModel.h"
#include "GameConstants.h"
#include "BallController.h"
#include "../common/Log.h"

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
    //Log::get_instance()->info("X: " +std::to_string(x));
    //Log::get_instance()->info("Y: " +std::to_string(y));


}

int BallModel::getXCorrection() {
    double initialcorrection=23.0;

    double multiplier=cos(angleToUse*PI/180.0);
    //std::cout<<"Angle:"<<angleToUse<<" Multiplicador:  "<<multiplier<<std::endl;

    double correction=multiplier*25;
    return initialcorrection+correction;
}

int BallModel::getYCorrection() {
    double initialcorrection=10.0;

    double multiplier=sin(angleToUse*PI/180.0);
    //std::cout<<"Angle:"<<angleToUse<<" Multiplicador:  "<<multiplier<<std::endl;

    double correction=multiplier*25;
    return initialcorrection+correction;

}

void BallModel::setAngle(double d) {
    angle=d;
    angleToUse=d;
    if(angleToUse>=90){
        angleToUse-=90;
    }else{
        angleToUse+=270;
    }
    correctPosition();

}

BallState BallModel::getState() {
    return state;
}

void BallModel::setState(BallState aState) {
    state=aState;

}

void BallModel::kick() {

    double multiplierX=cos(angleToUse*PI/180.0);
    double multiplierY=sin(angleToUse*PI/180.0);
    double xVel=250*multiplierX;
    double yVel=250*multiplierY;
    setVelX(xVel);
    setVelY(yVel);
}

void BallModel::setVelX(double d) {
    velX=d;

}

void BallModel::setVelY(double vel) {
    velY=vel;
}

void BallModel::update(double dt, int x_limit, int y_limit) {
    x += velX * dt;
    y += velY * dt;
    if ((y + this->getHeight()) > y_limit) { //limite de abajo
        y = y_limit - this->getHeight();
    }
    else if (this->y < 0) { // limite de arriba
        this->y = 0;
        velX=0;
        velY=0;
    }

    if ((x + this->getWidth()) > x_limit) { //limite de abajo
        x = x_limit - this->getWidth();
    }
    else if (this->x < 0) { // limite de arriba
        this->x = 0;
        velX=0;
        velY=0;
    }

}

int BallModel::getAngle() {
    return angleToUse;
}



