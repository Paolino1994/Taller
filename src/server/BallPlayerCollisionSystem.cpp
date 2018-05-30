#include "BallPlayerCollisionSystem.h"

#include <iostream>

BallPlayerCollisionSystem::BallPlayerCollisionSystem(World & world):
	world(world)
{}

BallPlayerCollisionSystem::~BallPlayerCollisionSystem()
{
}

void BallPlayerCollisionSystem::process(double dt)
{
	std::vector<PlayerController*>& playerControllers = world.getPlayerControllers();
	BallModel& ballModel = world.getBall().getModel();


	// BallModel::update (INICIAL)
	for (PlayerController* controller : playerControllers) {
		if (controller->getModel()->getHasControlOfTheBall()) {
			auto player = controller->getModel();
			ballModel.setX(player->getX());
			ballModel.setY(player->getY());
			ballModel.setAngle(player->getAngle());
			//return;
		}
	}

	// BallController::calculateCollision
	int x = ballModel.getX();
	int y = ballModel.getY();
	double z = ballModel.getZ();
	int i = 0;
	for (PlayerController* controller : playerControllers) {
		if (controller->getModel()->isColisionable) {
			int xPlayer = controller->getModel()->getCenterX();
			int yPlayer = controller->getModel()->getCenterY();
			if (abs(x - xPlayer)<20 && abs(y - yPlayer)<20 && z<2) {
				if (!controller->getModel()->getHasControlOfTheBall()) {
					Log::get_instance()->debug("Colision, un nuevo jugador toma la pelota");
					controller->getModel()->setHasControlOfTheBall(true);
					ballModel.setVelX(controller->getModel()->getVelX());
					ballModel.setVelY(controller->getModel()->getVelY());
					ballModel.setZ(0);
					ballModel.setVelZ(0);
					//controller->getModel()->setAngle(-90+ballModel.getAngle());

					// BallController::changeController(i, playerControllers);
					int counter = 0;
					for (PlayerController* controller : playerControllers) {
						if (i == counter) {
							controller->getModel()->setHasControlOfTheBall(true);
							//std::cout<<"Agarro la pelota"<< "Ball VelX: "<<ballModel.getVelX()<<" Ball VelY: "<<ballModel.getVelY()<<std::endl;
						}
						else {
							controller->getModel()->setHasControlOfTheBall(false);
						}
						counter++;
					}
				}
			}
		}

		i++;
	}

	// World::updateBallController()

	PlayerController* priorController = NULL;
	PlayerController* currentController = NULL;
	i = 0;
	for (auto player : playerControllers) {
		
		if (!player->isControllable()) {
			priorController = player;
			//std::cout<<"PLAYER"<<std::to_string(i)<<std::endl;
		}
		else {
			if (player->hasControlOfTheBall()) {
				currentController = player;
				//std::cout<<"New Controller"<<std::to_string(i)<<std::endl;
			}
		}
		if (priorController != NULL && currentController != NULL && priorController->getModel()->getTeam() == currentController->getModel()->getTeam()) {
			//std::cout<<"ENTRE"<<priorController->getModel()->getTeam()<<currentController->getModel()->getTeam()<<std::endl;
			priorController->swap(currentController);
			priorController = NULL;
			currentController = NULL;
			//controlCounter = 0;
		}
		i++;

	}
	//std::cout<<"ENTRE"<<priorController->getModel()->getTeam()<<currentController->getModel()->getTeam()<<std::endl;

	if (priorController != NULL && currentController != NULL && priorController->getModel()->getTeam() == currentController->getModel()->getTeam()) {
		//std::cout<<"ENTRE"<<priorController->getModel()->getTeam()<<currentController->getModel()->getTeam()<<std::endl;
		priorController->swap(currentController);
		//controlCounter = 0;
	}
	
}
