#include "BallPlayerCollisionSystem.h"

#include <iostream>

BallPlayerCollisionSystem::BallPlayerCollisionSystem(World & world):
	world(world)
{}

BallPlayerCollisionSystem::~BallPlayerCollisionSystem()
{
}

void BallPlayerCollisionSystem::process(double dt)
// TODO: REFACTOR
{
	last_steal_time += dt;
	std::vector<PlayerController*>& playerControllers = world.getPlayerControllers();
	BallModel& ballModel = world.getBall().getModel();

	char ball_player_role = 'M';
	int ball_player_team = BALL_WITHOUT_CONTROLLER;

	// BallModel::update (INICIAL)
	for (PlayerController* controller : playerControllers) {
		if (controller->getModel()->getHasControlOfTheBall()) {
			auto player = controller->getModel();
			if (!player->isGoalKeeper() || !player_is_in_his_area(player, (int)player->getTeam())) {
				ballLockedByGoalKeeper = false;
			}
			ballModel.setX(player->getX());
			ballModel.setY(player->getY());
			ballModel.setAngle(player->getAngle());
			ball_player_role = controller->getModel()->getRole();
			ball_player_team = (int)controller->getModel()->getTeam();
			break;
			//return;
		}
	}

	if (ball_player_team == BALL_WITHOUT_CONTROLLER) {
		ballLockedByGoalKeeper = false;
	}

	// BallController::calculateCollision
	int i = 0;
	if (!ballLockedByGoalKeeper) {
		for (PlayerController* controller : playerControllers) {
			if (controller->getModel()->isColisionable && !controller->getModel()->getHasControlOfTheBall()) {
				char rolePlayer = controller->getModel()->getRole();
				int teamPlayer = (int)controller->getModel()->getTeam();
				if (ball_player_team == BALL_WITHOUT_CONTROLLER || ((last_steal_time) > TIME_TO_NEXT_STEAL &&
						ball_player_team != teamPlayer)){	
					if (this->tries_to_recover(ballModel, controller->getModel())){
						Log::get_instance()->debug("Colision, un nuevo jugador intenta tomar la pelota");
						if (ball_player_team == BALL_WITHOUT_CONTROLLER || 
								recovers(rolePlayer, controller->getModel()->isSweeping(), ball_player_role)){
			
							Log::get_instance()->debug("Colision, un nuevo jugador toma la pelota");
							if (ball_player_team != BALL_WITHOUT_CONTROLLER){
								last_steal_time = 0;
							}
							controller->getModel()->setHasControlOfTheBall(true);
							GameManager::get_instance().setLastBallControlUser(controller->getUserId());
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
									GameManager::get_instance().setLastBallControlUser(controller->getUserId());
									////std::cout<<"Agarro la pelota"<< "Ball VelX: "<<ballModel.getVelX()<<" Ball VelY: "<<ballModel.getVelY()<<std::endl;
								}
								else {
									controller->getModel()->setHasControlOfTheBall(false);
								}
								counter++;
							}

							if (controller->getModel()->isGoalKeeper()) {
								int side;
								if ((Team)teamPlayer == Team::HOME) {
									side = GameManager::get_instance().getHomeDefends();
								} else {
									side = GameManager::get_instance().getAwayDefends();
								}

								if (player_is_in_his_area(controller->getModel(), side)) {
									ballLockedByGoalKeeper = true;
									//EventQueue::get().push(std::make_shared<GoalKickEvent>((Team)teamPlayer, side));
								}
							}

						} else {
							Log::get_instance()->debug("Colision, el nuevo jugador fallo en su intento de tomar la pelota");
						}
					}
				}
			}

			i++;
		}
	}

	// World::updateBallController()

	PlayerController* priorController = NULL;
	PlayerController* currentController = NULL;
	i = 0;
	for (auto player : playerControllers) {
		
		if (!player->isControllable()) {
			priorController = player;
			////std::cout<<"PLAYER"<<std::to_string(i)<<std::endl;
		}
		else {
			if (player->hasControlOfTheBall()) {
				currentController = player;
				////std::cout<<"New Controller"<<std::to_string(i)<<std::endl;
			}
		}
		if (priorController != NULL && currentController != NULL && priorController->getModel()->getTeam() == currentController->getModel()->getTeam()) {
			////std::cout<<"ENTRE"<<priorController->getModel()->getTeam()<<currentController->getModel()->getTeam()<<std::endl;
			priorController->swap(currentController);
			priorController = NULL;
			currentController = NULL;
			//controlCounter = 0;
		}
		i++;

	}
	////std::cout<<"ENTRE"<<priorController->getModel()->getTeam()<<currentController->getModel()->getTeam()<<std::endl;

	if (priorController != NULL && currentController != NULL && priorController->getModel()->getTeam() == currentController->getModel()->getTeam()) {
		////std::cout<<"ENTRE"<<priorController->getModel()->getTeam()<<currentController->getModel()->getTeam()<<std::endl;
		priorController->swap(currentController);
		//controlCounter = 0;
	}
}


bool BallPlayerCollisionSystem::tries_to_recover(BallModel& ball_model, PlayerModel* player_model)
{
	int radius = -1;
	int x_ball = ball_model.getX();
	int y_ball = ball_model.getY();
	double z_ball = ball_model.getZ();
	int x_player = player_model->getCenterX();
	int y_player = player_model->getCenterY();
	char role_player = player_model->getRole();
	
	switch(role_player){
		case 'G':
		case 'g':
			radius = G_RADIUS;
			break;
		case 'D':
		case 'd':
			radius = D_RADIUS;
			break;
		case 'M':
		case 'm':
			radius = M_RADIUS;
			break;
		case 'F':
		case 'f':
			radius = F_RADIUS;
			break;
	}

	if (player_model->isSweeping()) {
		radius *= 1.25;
	}

	return (sqrt(pow((double)(x_ball - x_player), 2) + pow((double)(y_ball - y_player), 2)) < radius && z_ball < 1.4);
}


bool BallPlayerCollisionSystem::recovers(char o_player_role, bool o_player_sweeping, char ball_player_role){
	srand(time(NULL));

	int ball_player_points = 0;
	int total_points = 0;
	switch(o_player_role){
		case 'G':
		case 'g':
			total_points += G_POINTS_RETRIEVE;
			break;
		case 'D':
		case 'd':
			total_points += D_POINTS_RETRIEVE;
			break;
		case 'M':
		case 'm':
			total_points += M_POINTS_RETRIEVE;
			break;
		case 'F':
		case 'f':
			total_points += F_POINTS_RETRIEVE;
			break;
	}
	if (o_player_sweeping){
		total_points *= 1.5;
	}

	switch(ball_player_role){
		case 'G':
		case 'g':
			total_points += G_POINTS_HOLD;
			ball_player_points = G_POINTS_HOLD;
			break;
		case 'D':
		case 'd':
			total_points += D_POINTS_HOLD;
			ball_player_points = D_POINTS_HOLD;
			break;
		case 'M':
		case 'm':
			total_points += M_POINTS_HOLD;
			ball_player_points = M_POINTS_HOLD;
			break;
		case 'F':
		case 'f':
			total_points += F_POINTS_HOLD;
			ball_player_points = F_POINTS_HOLD;
			break;		
	}
	float result = rand() % total_points;
	return result > ball_player_points;
}


bool BallPlayerCollisionSystem::player_is_in_his_area(PlayerModel* player_model, int side) {
	int x_player = player_model->getCenterX();
	int y_player = player_model->getCenterY();
	
	int top_y = YAML::WORLD_HEIGHT * 2 / 3;
	int bottom_y = YAML::WORLD_HEIGHT * 1 / 3;
	int left_x;
	int right_x;
	if (side == FIELD_POSITION::LEFT){
		left_x = 0;
		right_x = YAML::WORLD_WIDTH / 5;
	} else {
		left_x = YAML::WORLD_WIDTH * 4 / 5;
		right_x = YAML::WORLD_WIDTH;
	}

	if (x_player >= left_x && x_player <= right_x && y_player >= bottom_y && y_player <= top_y) {
		return true;
	}
	return false;
}
