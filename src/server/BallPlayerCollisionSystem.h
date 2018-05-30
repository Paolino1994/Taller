#pragma once
#include "System.h"
#include "World.h"

class BallPlayerCollisionSystem : public System
{
private:
	World & world;
public:
	BallPlayerCollisionSystem(World& world);
	virtual ~BallPlayerCollisionSystem();

	virtual void process(double dt); // quizas el tiempo no deberia estar, pero lo dejamos igual
};

