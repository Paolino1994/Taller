//
// Created by federico on 30/04/18.
//
#include <cmath>
#include <iostream>

#include "BallModel.h"
#include "BallController.h"
#include "common/Log.h"

#define PI 3.14159265


BallModel::BallModel(double kickOff_x, double kickOff_y, int X, int Y):
	Entity(kickOff_x, kickOff_y) {
    x=X;
    y=Y;
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
    return 20; // TODO: pasarlos desde un ball_data_t o algo asi
}

int BallModel::getHeight() {
    return 20; // TODO: pasarlos desde un ball_data_t o algo asi
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
	double xVel = 750 * multiplierX;
	double yVel = 750 * multiplierY;
    setVelX(xVel);
    setVelY(yVel);
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
void BallModel::update(double dt, int x_limit, int y_limit, std::vector<PlayerController*>& playerControllers) {

	for (PlayerController* controller : playerControllers) {
		if (controller->getModel()->getHasControlOfTheBall()) {
			auto player = controller->getModel();
			this->setX(player->getX());
			this->setY(player->getY());
			this->setAngle(player->getAngle());
			return;
		}
	}

	/*
	* ACA SE PODRIA HACE UN CHEQEUO DE CERCANIA CON RESPECTO A MODELS DE LOS JUGADORES
	* --> SI ESTOY A x_diff, y_diff cerca, entonces seteo a ese playerModel.setHasControlOfTheBall(true)
	* --> ojo! si hay varios cerca!
	*/

	x += velX * dt;
	y += velY * dt;
	if ((y + this->getHeight()) > y_limit) { //limite de abajo
		y = y_limit - this->getHeight();
		velX = 0;
		velY = 0;
	}
	else if (this->y < 0) { // limite de arriba
		this->y = 10;
		velX = 0;
		velY = 0;
	}

	if ((x + this->getWidth()) > x_limit) { //limite de abajo
		x = x_limit - this->getWidth();
		velX = 0;
		velY = 0;
	}
	else if (this->x < 0) { // limite de arriba
		this->x = 25;
		velX = 0;
		velY = 0;
	}
	setState();
	//std::cout<<"Ball VelX: "<<getVelX()<<" Ball VelY: "<<getVelY()<<std::endl;
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



