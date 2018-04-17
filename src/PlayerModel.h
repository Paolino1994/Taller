//
// Created by federico on 11/04/18.
//

#ifndef UNTITLED1_PLAYERMODEL_H
#define UNTITLED1_PLAYERMODEL_H

#include <vector>

#include "GameConstants.h"
#include "Entity.h"

class PlayerModel: public Entity {

public:

    PlayerModel(const player_data_t player_data, double initial_x, double initial_y);

	virtual int getWidth();

	virtual int getHeight();

    double getAngle();

    PlayerState getState();

    double getVelX();

    double getVelY();

    double getMaxVelY();

    double getMaxVelX();
	
    bool getIsControlledByHuman();
    
    void setIsControlledByHuman(bool controlled);

    void changeVelY(double d);
    void changeVelX(double d);

    void setVelY(double d);

    void setVelX(double d);

    void sweep();

    void kick();

    void sprint();
    void stopSprinting();

    void update(double dt, int x_limit, int y_limit);

	const int initial_x;
	const int initial_y;

private:
	
	const std::vector<int> widths;
	const std::vector<int> heights;
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
    double sweepVelY; 
    const double kickDuration;
    double kickTime;
	double kickVelX; // para saber que velX poner cuando termina el kick
	double kickVelY; // para saber que velY poner cuando termina el kick
	// Sprint
	const double sprintVelocityMultiplier;
	double velocityMultiplier;

    bool isControlledByHuman = false;

};



#endif //UNTITLED1_PLAYERMODEL_H
