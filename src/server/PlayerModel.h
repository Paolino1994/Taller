#pragma once

#include <vector>

#include "common/GameConstants.h"
#include "common/Log.h"
#include "Entity.h"
#include "BallModel.h"


class PlayerModel: public Entity {

public:

    PlayerModel(Team team, const player_data_t player_data, double initial_x, double initial_y, int kickOffX, int kickOffY, int distance_x, int distance_y);
    PlayerModel(Team team, const player_data_t player_data, double initial_x, double initial_y);


	virtual int getWidth();

	virtual int getHeight();

	int getInitial_x();

	int getInitial_y();

    void setInitial_x(int _x);

	void setInitial_y(int _y);

	Team getTeam();

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

	// TODO: Evaluar si conviene tener una referencia a la pelota cuando el PlayerModel la tiene bajo su control
    void kick(BallModel& ballModel,double distance);

    void sprint();
    void stopSprinting();

    void update(double dt, int x_limit, int y_limit);

    int getKickOff_x();
    int getKickOff_y();

	// TODO: Evaluar si conviene tener una referencia a la pelota cuando el PlayerModel la tiene bajo su control
	void setHasControlOfTheBall(bool i);
	// TODO: Evaluar si conviene tener una referencia a la pelota cuando el PlayerModel la tiene bajo su control
	bool getHasControlOfTheBall();

	// TODO: Esto lo deberia poder hacer la pelota por su propia cuenta en el update!
    void changeBallState(BallModel& ballModel);

	// TODO: Evaluar si conviene tener una referencia a la pelota cuando el PlayerModel la tiene bajo su control
    void pass(PlayerModel *pModel, BallModel& ballModel);

	Player_ID getPlayerId();

	void setAngle(int i);

    bool isColisionable=true;

    void longPass(PlayerModel *pModel, BallModel &model);

	void kick(BallModel &model);

    int getDistance_x();
    int getDistance_y();

	bool isInChargeOfKickOff();

	void setInChargeOfKickOff(bool inCharge);

private:
    int initial_x;
	int initial_y;

    int kickOff_x;
    int kickOff_y;
    int distance_x = 100;
	int distance_y = 100;
	Team team;
	Player_ID playerId;
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

	bool hasControlOfTheBall=false;

    Log* log;

	static Player_ID nextPlayerId[static_cast<std::underlying_type<Team>::type>(Team::__LENGTH__)];

	static Player_ID getNextPlayerId(Team team);




	int colisionableCounter=10;

    void kickHigh(double model, BallModel &ballModel);

	double getAngleToGoal();

	double getDistanceToGoal();

	// TODO: a mover la decisi�n a un objeto Team
	bool inChargeOfKickOff;
};
