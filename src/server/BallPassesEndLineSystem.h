#pragma once
#include "System.h"
#include "World.h"

class BallPassesEndLineSystem : public System
{
private:
	World & world;
public:
	BallPassesEndLineSystem(World& world);
	virtual ~BallPassesEndLineSystem();

	virtual void process(double dt); // quizas el tiempo no deberia estar, pero lo dejamos igual
};
