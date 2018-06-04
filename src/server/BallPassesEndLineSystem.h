#pragma once
#include "System.h"
#include "World.h"
#include "GameManager.h"
#include "common/GameConstants.h"


class BallPassesEndLineSystem : public System, public EventHandler
{
private:
	World & world;
	const PlayerModel* lastPlayerThatKickedTheBall;
public:
	BallPassesEndLineSystem(World& world);
	virtual ~BallPassesEndLineSystem();

	virtual void process(double dt); // quizas el tiempo no deberia estar, pero lo dejamos igual

	// Events to handle
	virtual void handle(KickEvent& e);
};
