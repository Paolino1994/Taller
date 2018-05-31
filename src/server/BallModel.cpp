//
// Created by federico on 30/04/18.
//
#include <cmath>
#include <iostream>

#include "BallModel.h"
#include "common/Log.h"
#include "common/GameConstants.h"

#define PI 3.14159265


BallModel::BallModel(double kickOff_x, double kickOff_y, int X, int Y):
	Entity(kickOff_x, kickOff_y) {
    x=X;
    y=Y;
	z=0;
	velX = 0;
	velY = 0;
    state=QUIESCENT;

}

BallModel::BallModel(double kickOff_x, double kickOff_y): 
	Entity(kickOff_x, kickOff_y),
	state(QUIESCENT),
	velX(0),
	velY(0)
{
}

int BallModel::getWidth() {
    return 20;//*(1+z); // TODO: pasarlos desde un ball_data_t o algo asi
}

int BallModel::getHeight() {
    return 20;//*(1+z); // TODO: pasarlos desde un ball_data_t o algo asi
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

void BallModel::setVelX(double d) {
	velX = d;
	setState();
}

void BallModel::setVelY(double vel) {
	velY = vel;
	setState();
}

// TODO: pasar PlayerModels en vez de PlayerControllers
void BallModel::update(double dt, int x_limit, int y_limit) {
	
	// Aplicar l�gica de movimiento y como mucho choques con fin del mundo
	// Los otros comportamientos con la pelota los deben hacer los sistemas

	velX*=0.995;
    velY*=0.995;
	x += velX * dt;
	y += velY * dt;
	if ((y + this->getHeight()) > y_limit) { //limite de abajo
		y = y_limit - this->getHeight();
		//velX = 0;
		velY = -velY;
	}
	else if (this->y < 0) { // limite de arriba
		this->y = 25;
		//velX = 0;
		velY = -velY;
	}

	if ((x + this->getWidth()) > x_limit) { //limite de abajo
		x = x_limit - this->getWidth();
		velX = -velX;
		//velY = 0;
        //velZ = 0;
	}
	else if (this->x < 0) { // limite de arriba
		this->x = 25;
		velX = -velX;
		//velY = 0;
		//velZ = 0;
	}

	if(z>0){
		double currentDistance = getCurrentDistanceToOriginal();
		if(currentDistance>=startDistance/2){
			heigthAngle=-1;
		}
	}


	if(heigthAngle>=0){
		z=z+velZ*dt;
	}else{
		if(z>0){
            z=z-velZ*dt;
		}else{
            z=0;
        }
	}
    /*double velMultiplier=0.999;//1-getCurrentDistanceToOriginal()/startDistance;
    if(getCurrentDistanceToOriginal()==0){
        velMultiplier=0.9;
    }*/
    //std::cout<<"startDistance "<<startDistance<<" current "<<getCurrentDistanceToOriginal()<<" multiplier "<<velMultiplier<<std::endl;


    /*if(velX>0){
        velX*=velMultiplier;
    }else{
        velX=0;
    }
    if(velY>0){
        velY*=velMultiplier;
    }else{
        velY=0;
    }
    if(velZ>0){
        velZ*=velMultiplier;
    }else{
        velZ=0;
    }*/
    if(abs(velX)<50 && abs(velY)<50){
        velX=0;
        velY=0;
    }
    //std::cout<<"Xvel: "<<std::to_string(velX)<<" Yvel: "<<std::to_string(velY)<<" Zvel: "<<std::to_string(velZ)<<std::endl;
	setState();
	//std::cout<<"Z: "<<z<<std::endl;
}

int BallModel::getAngle() {
    return angleToUse;
}

double BallModel::getVelX() {
	return velX;
}

double BallModel::getVelY() {
	return velY;
}

void BallModel::setState() {
	//std::cout<<"VELX: "<<velX<<" VelY: "<<velX<<std::endl;
	if (velX != 0 || velY != 0) {
		state = MOVING;
	}
	else {
		state = QUIESCENT;
	}
}

void BallModel::kick(double distance, int type) {
	double multiplierX=cos(angleToUse*PI/180.0);
	double multiplierY=sin(angleToUse*PI/180.0);
    double speed=getSpeed(distance);
	double xVel = speed * multiplierX;
	double yVel = speed * multiplierY;
    //double yVel = 750 * multiplierZ;
	setVelX(xVel);
	setVelY(yVel);
    setVelZ(10);
	originalX=this->getX();
	originalY=this->getY();
    if(type==HIGH){
        heigthAngle=45;
    }
	startDistance=distance;
    passType=type;
}

double BallModel::getCurrentDistanceToOriginal() {
	double distance=pow(pow(abs(originalX-x),2) + pow(abs(originalY-y),2),0.5);
	return distance;

}

void BallModel::setVelZ(double i) {
    velZ=i;

}

double BallModel::getZ() {
    return z;
}

void BallModel::setZ(double i) {
    z=i;
}

double BallModel::getSpeed(double distance) {
    if(distance*2>1000){
        return 1000;
    }
    return distance*2;
}

