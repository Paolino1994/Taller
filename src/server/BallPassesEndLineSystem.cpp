#include "BallPassesEndLineSystem.h"

#include <iostream>

#include "common/GameConstants.h"
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

		const int leftEndLineX = YAML::FIELD_X;
		const int goalBottomPost = YAML::GOAL[FIELD_POSITION::LEFT].y;
		const int goalTopPost = YAML::GOAL[FIELD_POSITION::LEFT].y + YAML::GOAL[FIELD_POSITION::LEFT].h;
		const int postWidth = YAML::POST_WIDTH;
		const int crossbarWidth = YAML::CROSSBAR_WIDTH;
		const int goalHeight = YAML::GOAL_HEIGTH;
		const int rigthEndLineX = YAML::FIELD_X + YAML::FIELD_WIDTH;


		int x = ballModel.getX();
		int y = ballModel.getY();
		double z = ballModel.getZ();
		// double z = ballModel.getZ();

		// condiciones de gol para FIELD_POSITION::LEFT
		if (x < leftEndLineX)
		{
			if (y > goalBottomPost - postWidth && y < goalTopPost + postWidth && z < goalHeight + crossbarWidth)  // Tambien podemos poner la validacion en eje z para el travesanio
			{
				if (y > goalBottomPost && y < goalTopPost && z < goalHeight)
				{
					std::cout << "GOOOL" << std::endl;

					Team goalTeam = (GameManager::get_instance().getHomeDefends() == FIELD_POSITION::LEFT) ? Team::AWAY : Team::HOME;

					world.setSetPiecePosition(Team::HOME, GameManager::get_instance().getHomeDefends(), SET_PIECE::KICKOFF);
					world.setSetPiecePosition(Team::AWAY, GameManager::get_instance().getAwayDefends(), SET_PIECE::KICKOFF);

					if(goalTeam == GameManager::get_instance().getTeamBallControl()) {
						GameManager::get_instance().goalScoredByUser();
						std::cout << "GOL DE TU EQUIPO" << std::endl;
					} else {
						std::cout << "GOL EN CONTRA" << std::endl;
					}

					EventQueue::get().push(std::make_shared<GoalEvent>(this->lastPlayerThatKickedTheBall, goalTeam));
				}
				else {
					EventQueue::get().push(std::make_shared<PostHitEvent>());
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

		// condiciones de gol para FIELD_POSITION::RIGHT
		if (x > rigthEndLineX) {
			if (y > goalBottomPost - postWidth && y < goalTopPost + postWidth && z < goalHeight + crossbarWidth) // Tambien podemos poner la validacion en eje z para el travesanio
					{
				if (y > goalBottomPost && y < goalTopPost && z < goalHeight ) {
					std::cout << "GOOOL" << std::endl;
					Team goalTeam = (GameManager::get_instance().getHomeDefends() == FIELD_POSITION::RIGHT) ? Team::AWAY : Team::HOME;
					world.setSetPiecePosition(Team::HOME, GameManager::get_instance().getHomeDefends(), SET_PIECE::KICKOFF);
					world.setSetPiecePosition(Team::AWAY, GameManager::get_instance().getAwayDefends(), SET_PIECE::KICKOFF);


					if(goalTeam == GameManager::get_instance().getTeamBallControl()) {
						GameManager::get_instance().goalScoredByUser();
						std::cout << "GOL DE TU EQUIPO" << std::endl;
					} else {
						std::cout << "GOL EN CONTRA" << std::endl;
					}


					EventQueue::get().push(std::make_shared<GoalEvent>(this->lastPlayerThatKickedTheBall, goalTeam));
				} else {
					std::cout << "PALO" << std::endl;
					EventQueue::get().push(std::make_shared<PostHitEvent>());
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


