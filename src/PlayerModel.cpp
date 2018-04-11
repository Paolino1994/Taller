//
// Created by federico on 11/04/18.
//

#include "PlayerModel.h"
#include <cmath>


PlayerModel::PlayerModel(const player_data_t player_data, double initial_x, double initial_y):
        Entity(initial_x, initial_y),
        velX(0),
        velY(0),
        state(STILL),
        angle(0.0),
        MAX_VEL_X(player_data.X_VELOCITY),
        MAX_VEL_Y(player_data.Y_VELOCITY)
{

}

void PlayerModel::update(double dt, int x_limit, int y_limit){
    using namespace std;
    PlayerState old_state = this->state;
    // Actualizar x:
    x += velX * dt;
    // Actualizar y:
    y += velY * dt;


    if((y + this->getHeight()) > y_limit ) { //limite de abajo
        y = y_limit - this->getHeight();
    }
    else if (this->y < 0) { // limite de arriba
        this->y = 0;
    }

    if ((x + this->getWidth()) > x_limit) { //limite de abajo
        x = x_limit - this->getWidth();
    }
    else if (this->x < 0) { // limite de arriba
        this->x = 0;
    }

    if (velX == 0.0 && velY == 0.0) {
        this->state = STILL;
        // mantenemos el angulo anterior
    }
    else {
        this->state = RUNNING;
        // angle con 0 apunta para arriba, 180 abajo, 360 arriba, lo pasado de 360 o 0 lo modulea SDL2
        this->angle = (atan2(this->velY, this->velX) * 180 / M_PI) + 90;
    }


}

int PlayerModel::getWidth()
{
    //return mAnimations[this->state].getDestinationWidth();
    return 20;
}

int PlayerModel::getHeight()
{
    //return mAnimations[this->state].getDestinationHeight();
    return 20;
}

double PlayerModel::getAngle() {
    return angle;

}

PlayerState PlayerModel::getState() {
    return state;

}



double PlayerModel::getVelX() {
    return velX;
}

double PlayerModel::getVelY() {
    return velY;
}

double PlayerModel::getMaxVelY() {
    return MAX_VEL_Y;
}

double PlayerModel::getMaxVelX() {
    return MAX_VEL_X;
}

void PlayerModel::setVelY(double d) {
    velY=d;
}

void PlayerModel::setVelX(double d) {
    velX=d;
}

PlayerModel::PlayerModel():
        Entity(0, 0),
        velX(0),
        velY(0),
        state(STILL),
        angle(0.0),
        MAX_VEL_X(0),
        MAX_VEL_Y(0) {

}


