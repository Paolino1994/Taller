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

    double getAngle();

    PlayerState getState();

    double getVelX();

    double getVelY();

    double getMaxVelY();

    double getMaxVelX();

	void changeVelY(double d);
	void changeVelX(double d);

    void setVelY(double d);

    void setVelX(double d);

	void sweep();

	void sprint();
	void stopSprinting();

    void update(double dt, int x_limit, int y_limit);

private:
    double velX;
    double velY;
    PlayerState state;
    double angle;
    const int MAX_VEL_X;
    const int MAX_VEL_Y;
	// Todo esto de aca para abajo seria para una clase SweepState (ponele) que implemente PlayerState
	const double sweepDuration;
	double sweepTime;
	double sweepVelX; // para saber que velX poner cuando termina el sweep
	double sweepVelY; // para saber que velY poner cuando termina el sweep
	// Sprint
	const double sprintVelocityMultiplier;
	double velocityMultiplier;

    int getWidth();

    int getHeight();
};



#endif //UNTITLED1_PLAYERMODEL_H
