#pragma once
#include <cstdlib>
#include "System.h"
#include "World.h"

#define G_POINTS_RETRIEVE 100
#define G_POINTS_HOLD 50
#define G_RADIUS 40
#define D_POINTS_RETRIEVE 80
#define D_POINTS_HOLD 30
#define D_RADIUS 30
#define M_POINTS_RETRIEVE 70
#define M_POINTS_HOLD 30
#define M_RADIUS 25
#define F_POINTS_RETRIEVE 50
#define F_POINTS_HOLD 30
#define F_RADIUS 20

#define BALL_WITHOUT_CONTROLLER -1
#define TIME_TO_NEXT_STEAL 0.3

class BallPlayerCollisionSystem : public System
{
private:
	World & world;
	double last_steal_time = 0;

	bool tries_to_recover(BallModel& ball_model, PlayerModel* player_model);
	bool recovers(char o_player_role, bool o_player_sweeping, char ball_player_role);
public:
	BallPlayerCollisionSystem(World& world);
	virtual ~BallPlayerCollisionSystem();

	virtual void process(double dt); // quizas el tiempo no deberia estar, pero lo dejamos igual
};

