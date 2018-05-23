//
// Created by federico on 30/04/18.
//
#pragma once

#include "Entity.h"
#include "common/GameConstants.h"
#include "PlayerController.h"

class BallModel: public Entity {

public:
    BallModel(double kickOff_x, double kickOff_y, int X, int Y);

    BallModel(double kickOff_x, double kickOff_y);

	double getVelX();

	double getVelY();

    int getWidth();

    int getHeight();

    void setX(double d);

    void setY(double d);

    void correctPosition();

    int getXCorrection();

    int getYCorrection();

    void setAngle(double d);

    BallState getState();

    void setState(BallState state);

    void kick();

	// TODO: pasar PlayerModels en vez de PlayerControllers
    void update(double dt, int x_limit, int y_limit, std::vector<PlayerController*>& playerControllers);

    int getAngle();

	void setVelX(double d);

	void setVelY(double vel);

	void setState();

    void kick(double distance, int type);

    static const int GOAL=3;

    static const int HIGH=2;

    static const int LOW=1;

	double getZ();

    void setZ(double i);

    void setVelZ(double i);

private:
    double angle=0;

    BallState state;

    double velX;

    double velY;

    double velZ;

    double z;

    double angleToUse;

    double heigthAngle;

    int originalX;

    int originalY;

    double startDistance;

    double getCurrentDistanceToOriginal();

    int passType=0;


    double getSpeed(double distance);
};
