#include "BallPassesEndLineSystem.h"

#include <iostream>

#include "common/EventQueue.h"

BallPassesEndLineSystem::BallPassesEndLineSystem(World & world):
	world(world),
	lastPlayerThatKickedTheBall(nullptr)
{
	this->registerTo(EventID::KICK);
}

BallPassesEndLineSystem::~BallPassesEndLineSystem()
{
}

void BallPassesEndLineSystem::process(double dt)
{
	// hago estos chequeos solo si la pelota esta en juego!!!
	if (GameManager::get_instance().isBallInPlay()) {
		BallModel& ballModel = world.getBall().getModel();

		int leftEndLineX = 60;
		int goalBottomPost = 420;
		int goalTopPost = 620;
		int postWidth = 30;
		//int rigthEndLineX = 30;

		int x = ballModel.getX();
		int y = ballModel.getY();
		// double z = ballModel.getZ();

		if (x < leftEndLineX)
		{
			if (y > goalBottomPost - postWidth && y < goalTopPost + postWidth)  // Tambien podemos poner la validacion en eje z para el travesanio
			{
				if (y > goalBottomPost && y < goalTopPost)
				{
					std::cout << "GOOOL" << std::endl;
					//TEMP -> TODO: ojo que el que metio el gol quizas no la pateo, y entro caminando con la pelota por el arco
					//TEMP -> TODO: chequear gol en base al arco y posicion del equipo
					Team goalTeam = this->lastPlayerThatKickedTheBall != nullptr ? this->lastPlayerThatKickedTheBall->getTeam() : Team::HOME;
					EventQueue::get().push(std::make_shared<GoalEvent>(this->lastPlayerThatKickedTheBall, goalTeam));
				}
				else {
					std::cout << "PALO" << std::endl;
					ballModel.setVelX(-ballModel.getVelX()); // Hago rebotar la pelota en el palo, le cambio el angulo en x
				}
			}
			else {
				std::cout << "Salio por el costado izquierdo" << std::endl;
			}
		}
	}
}

void BallPassesEndLineSystem::handle(KickEvent & e)
{
	this->lastPlayerThatKickedTheBall = &e.player;
}


