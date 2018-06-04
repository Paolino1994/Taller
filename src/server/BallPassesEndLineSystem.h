#pragma once
#include "System.h"
#include "World.h"
#include "GameManager.h"
#include "common/GameConstants.h"


class BallPassesEndLineSystem : public System
{
private:
	World & world;
	bool gol = false;
public:
	BallPassesEndLineSystem(World& world);
	virtual ~BallPassesEndLineSystem();

	virtual void process(double dt); // quizas el tiempo no deberia estar, pero lo dejamos igual
};
