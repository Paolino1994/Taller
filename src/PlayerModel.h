//
// Created by federico on 11/04/18.
//

#ifndef UNTITLED1_PLAYERMODEL_H
#define UNTITLED1_PLAYERMODEL_H


#include "GameConstants.h"
#include "Entity.h"

class PlayerModel: public Entity {

public:
    PlayerModel();

    PlayerModel(const player_data_t player_data, double initial_x, double initial_y);

    //PlayerModel();


    double getAngle();

    PlayerState getState();

    double getVelX();

    double getVelY();

    double getMaxVelY();

    double getMaxVelX();

    void setVelY(double d);

    void setVelX(double d);

    void update(double dt, int x_limit, int y_limit);

private:
    double velX;
    double velY;
    PlayerState state;
    double angle;
    const int MAX_VEL_X;
    const int MAX_VEL_Y;

    int getWidth();

    int getHeight();
};



#endif //UNTITLED1_PLAYERMODEL_H
