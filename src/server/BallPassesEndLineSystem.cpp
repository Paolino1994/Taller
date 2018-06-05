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

		int leftEndLineX = 50;
		int goalBottomPost = 347;
		int goalTopPost = 550;
		int postWidth = 20;
		int rigthEndLineX = 1628;


		int x = ballModel.getX();
		int y = ballModel.getY();
		// double z = ballModel.getZ();

		// condiciones de gol para Team::AWAY
		if (x < leftEndLineX)
		{
			if (y > goalBottomPost - postWidth && y < goalTopPost + postWidth)  // Tambien podemos poner la validacion en eje z para el travesanio
			{
				if (y > goalBottomPost && y < goalTopPost)
				{
					std::cout << "GOOOL" << std::endl;



					//TEMP -> TODO: ojo que el que metio el gol quizas no la pateo, y entro caminando con la pelota por el arco
					//TEMP -> TODO: chequear gol en base al arco y posicion del equipo

					Team goalTeam = (GameManager::get_instance().getHomeDefends() == FIELD_POSITION::LEFT) ? Team::AWAY : Team::HOME;
					world.setSetPiecePosition(Team::HOME, GameManager::get_instance().getHomeDefends(), SET_PIECE::KICKOFF);
					world.setSetPiecePosition(Team::AWAY, GameManager::get_instance().getAwayDefends(), SET_PIECE::KICKOFF);
					// Esto me parece que esta mal, porque puedo hacer goles en contra y me sumo un gol
					// Team goalTeam = this->lastPlayerThatKickedTheBall != nullptr ? this->lastPlayerThatKickedTheBall->getTeam() : Team::AWAY;
					EventQueue::get().push(std::make_shared<GoalEvent>(this->lastPlayerThatKickedTheBall, goalTeam));
				}
				else {
					std::cout << "PALO" << std::endl;
					ballModel.setVelX(-ballModel.getVelX()); // Hago rebotar la pelota en el palo, le cambio el angulo en x
				}
			}
			else {
				std::cout << "Salio por el costado izquierdo" << std::endl;
				Team goalKickTeam = (GameManager::get_instance().getHomeDefends() == FIELD_POSITION::LEFT) ? Team::HOME : Team::AWAY;
				world.setSetPiecePosition(Team::HOME, GameManager::get_instance().getHomeDefends(), SET_PIECE::GOALKICK);
				world.setSetPiecePosition(Team::AWAY, GameManager::get_instance().getAwayDefends(), SET_PIECE::GOALKICK);
				EventQueue::get().push(std::make_shared<GoalKickEvent>(goalKickTeam, FIELD_POSITION::LEFT));
			}
		}

		// condiciones de gol para Team::HOME
		if (x > rigthEndLineX) {
			if (y > goalBottomPost - postWidth && y < goalTopPost + postWidth) // Tambien podemos poner la validacion en eje z para el travesanio
					{
				if (y > goalBottomPost && y < goalTopPost) {
					std::cout << "GOOOL" << std::endl;
					// Aca estoy probando el GameManager, no es para nada definitivo que sea asi
//					GameManager::get_instance()->addGoal(Team::HOME); // Obvio que no es asi porque falta la logica del cambio de lado
					// Team goalTeam = this->lastPlayerThatKickedTheBall != nullptr ? this->lastPlayerThatKickedTheBall->getTeam() : Team::HOME;

					Team goalTeam = (GameManager::get_instance().getHomeDefends() == FIELD_POSITION::RIGHT) ? Team::AWAY : Team::HOME;
					world.setSetPiecePosition(Team::HOME, GameManager::get_instance().getHomeDefends(), SET_PIECE::KICKOFF);
					world.setSetPiecePosition(Team::AWAY, GameManager::get_instance().getAwayDefends(), SET_PIECE::KICKOFF);
					EventQueue::get().push(std::make_shared<GoalEvent>(this->lastPlayerThatKickedTheBall, goalTeam));
				} else {
					std::cout << "PALO" << std::endl;
					ballModel.setVelX(-ballModel.getVelX()); // Hago rebotar la pelota en el palo, le cambio el angulo en x
				}
			} else {
				std::cout << "Salio por el costado derecho" << std::endl;
				Team goalKickTeam = (GameManager::get_instance().getHomeDefends() == FIELD_POSITION::RIGHT) ? Team::HOME : Team::AWAY;
				world.setSetPiecePosition(Team::HOME, GameManager::get_instance().getHomeDefends(), SET_PIECE::GOALKICK);
				world.setSetPiecePosition(Team::AWAY, GameManager::get_instance().getAwayDefends(), SET_PIECE::GOALKICK);
				EventQueue::get().push(std::make_shared<GoalKickEvent>(goalKickTeam, FIELD_POSITION::RIGHT));
			}
		}

	}
}

void BallPassesEndLineSystem::handle(KickEvent & e)
{
	this->lastPlayerThatKickedTheBall = &e.player;
}


