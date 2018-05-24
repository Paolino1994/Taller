#include "World.h"
#include "BallController.h"
#include "TeamFactory.h"
#include "PlayerControllerHuman.h"
#include "PlayerControllerAI.h"
#include <algorithm>
#include <iostream>

World::World(int width, int height, std::map<const std::string, Animation> ballAnimMapper):
	ball(BallController(width/2, height/2, ballAnimMapper)),
    entities(std::vector<Entity*>()),
	pControllers(std::vector<PlayerController*>()),
    width(width),
    height(height)
{
}

World::~World()
{
	for (auto controllers : playerControllers) {
		for (PlayerController* controller : controllers) {
			delete controller->getModel();
			delete controller->getView();
			delete controller;
		}
	}
}

void World::createTeam(Team team, int defenders, int midfielders, int forwards, player_data_t playerData, std::map<const std::string, Animation>& animMapper)
{
	size_t teamIndex = static_cast<std::underlying_type<Team>::type>(team);
	if (team != Team::__LENGTH__ && playerControllers[teamIndex].size() == 0) {
		TeamFactory teamFactory(playerData);
		teamFactory.create(defenders, midfielders, forwards, team, this->getWidth(), this->getHeight());
		teamFactory.add_view(animMapper);
		teamFactory.fill(playerControllers[teamIndex]);

		for (auto controller : playerControllers[teamIndex]) {
			pControllers.push_back(controller);
		}
	}
	else {
		Log::get_instance()->info("Intentaron recrear un equipo, no lo permito y me quedo con el anterior");
	}
}

bool World::noOneHasControlOfTheBall(std::vector<PlayerController*>& teamControllers){
    for(PlayerController* player : teamControllers){
        if(player->hasControlOfTheBall()){
            return false;
        }
    }
    return true;
}

PlayerController* World::injectHumanController(Team team, User_ID userId)
{
    std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(team)];

    for (size_t i = teamControllers.size() - 1; i >= 0 ; i--)
    {
        if (teamControllers[i]->isControllable()) {
            PlayerController* other = teamControllers[i];
            PlayerControllerHuman* human = new PlayerControllerHuman(other->getModel(), other->getView(), *this, userId);
            teamControllers[i] = human;
            human->getModel()->setIsControlledByHuman(true);
            //if(noOneHasControlOfTheBall(teamControllers)){
            //    human->getModel()->setHasControlOfTheBall(true);
            //}
            delete other;
            return human;
        }
    }

    return nullptr;
}

bool World::playerIsOnRange(PlayerController* cont,PlayerController* controllerToSwap){
    //PlayerModel* model=cont->getModel();
    int x=ball.getModel().getX();
    int y=ball.getModel().getY();
    int xCon=cont->getModel()->getCenterX();
    int yCon=cont->getModel()->getCenterY();
    //std::cout<<"BALL X:"<<x<<" Y:"<<y<<std::endl;
    //std::cout<<"X:"<<xCon<<" Y:"<<yCon<<std::endl;
    //int xRange=getRangeToChange(x,200);
    //int yRange=getRangeToChange(y,150);
    if(abs(x-xCon)<YAML::SCREEN_WIDTH/2 && abs(y-yCon)<YAML::SCREEN_HEIGHT/2){
        //std::cout<<"vale Controllador "<<std::endl;
        return true;
    }
    return false;

}

PlayerController* World::getPlayerToPass(PlayerController * controllerToSwap){
    Team team = controllerToSwap->getModel()->getTeam();
    std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(team)];

	ptrdiff_t index = std::distance(teamControllers.begin(), std::find(teamControllers.begin(), teamControllers.end(), controllerToSwap));

	if (index >= (long long)teamControllers.size()) {
		Log::get_instance()->info("El controller que se quiso swapear no esta en mi listado de controllers del equipo correspondiente");

	}

	size_t controllerToSwapIndex = index;
	//bool comparacion;
	index=-1;
	do
	{ // en el peor caso volvemos a el que estabamos controlando recien
		//++index;
		index++;
		if (index == (long long)teamControllers.size()) {
			index = controllerToSwapIndex;
			break;
		}
		if(teamControllers[index] != controllerToSwap && playerIsOnRange(teamControllers[index],controllerToSwap) /*&& teamControllers[index]->isControllable()*/){
			break;
		}
		/*comparacion=!playerIsOnRange(teamControllers[index],controllerToSwap);
		comparacion=(comparacion==(teamControllers[index] != controllerToSwap));
		comparacion=(comparacion==!teamControllers[index]->isControllable());*/
		//comparacion=comparacion==false;

	} //while (!playerIsOnRange(teamControllers[index],controllerToSwap) && teamControllers[index] != controllerToSwap && !teamControllers[index]->isControllable()); // sin chequeos de camara por ahora -> igual se sacaba para la fase 2
	while(true);
	return teamControllers[index];

}

PlayerController* World::getPlayerToPassLong(PlayerController * controllerToSwap){
	Team team = controllerToSwap->getModel()->getTeam();
	std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(team)];

	ptrdiff_t index = std::distance(teamControllers.begin(), std::find(teamControllers.begin(), teamControllers.end(), controllerToSwap));

	if (index >= (long long)teamControllers.size()) {
		Log::get_instance()->info("El controller que se quiso swapear no esta en mi listado de controllers del equipo correspondiente");

	}

	size_t controllerToSwapIndex = index;
	//bool comparacion;
	index=-1;
	do
	{ // en el peor caso volvemos a el que estabamos controlando recien
		//++index;
		index++;
		if (index == (long long)teamControllers.size()) {
			index = controllerToSwapIndex;
			break;
		}
		if(teamControllers[index] != controllerToSwap && !playerIsOnRange(teamControllers[index],controllerToSwap) /*&& teamControllers[index]->isControllable()*/){
			break;
		}
		/*comparacion=!playerIsOnRange(teamControllers[index],controllerToSwap);
		comparacion=(comparacion==(teamControllers[index] != controllerToSwap));
		comparacion=(comparacion==!teamControllers[index]->isControllable());*/
		//comparacion=comparacion==false;

	} //while (!playerIsOnRange(teamControllers[index],controllerToSwap) && teamControllers[index] != controllerToSwap && !teamControllers[index]->isControllable()); // sin chequeos de camara por ahora -> igual se sacaba para la fase 2
	while(true);
	return teamControllers[index];

}

bool World::ejectController(PlayerController * playerController, User_ID userId)
{
	std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(playerController->getModel()->getTeam())];

	for (size_t i = 0; i < teamControllers.size(); i++)
	{
		if (teamControllers[i] == playerController) {
			PlayerControllerAI* aiController = new PlayerControllerAI(playerController->getModel(), playerController->getView());
			teamControllers[i] = aiController;
			aiController->getModel()->setIsControlledByHuman(false);
			aiController->getModel()->setHasControlOfTheBall(false);
			delete playerController;
			return true;
		}
	}

	return false;
}

void World::swap(PlayerController * controllerToSwap)
{
	Team team = controllerToSwap->getModel()->getTeam();
	std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(team)];

	ptrdiff_t index = std::distance(teamControllers.begin(), std::find(teamControllers.begin(), teamControllers.end(), controllerToSwap));

	if (index >= (long long)teamControllers.size()) {
		Log::get_instance()->info("El controller que se quiso swapear no esta en mi listado de controllers del equipo correspondiente");
		return;
	}

	size_t controllerToSwapIndex = index;
	//std::cout<<"-----------------------------------------------------"<<std::endl;
	index=-1;
	do
	{ // en el peor caso volvemos a el que estabamos controlando recien
		//++index;
		index++;
		if (index == (long long)teamControllers.size()) {
			index = controllerToSwapIndex;
			break;
		}
		if(teamControllers[index] != controllerToSwap && playerIsOnRange(teamControllers[index],controllerToSwap) && teamControllers[index]->isControllable()){
			break;
		}
		/*comparacion=!playerIsOnRange(teamControllers[index],controllerToSwap);
		comparacion=(comparacion==(teamControllers[index] != controllerToSwap));
		comparacion=(comparacion==!teamControllers[index]->isControllable());*/
		//comparacion=comparacion==false;

	} //while (!playerIsOnRange(teamControllers[index],controllerToSwap) && teamControllers[index] != controllerToSwap && !teamControllers[index]->isControllable()); // sin chequeos de camara por ahora -> igual se sacaba para la fase 2
	while(true);
	if (teamControllers[index] != controllerToSwap) {
		controllerToSwap->swap(teamControllers[index]);
		// camera.follow(controlled->getEntity()); lo hacemos en el main
		auto temp = teamControllers[index];
		teamControllers[index] = controllerToSwap;
		teamControllers[controllerToSwapIndex] = temp;
	}
}



void World::addEntity(Entity* entity){
    entities.push_back(entity);
}



void World::addPlayerController(PlayerController* pController){
	pControllers.push_back(pController);
}


BallController& World::getBall()
{
	return ball;
}

std::vector<Entity*>& World::getEntities() {
    return entities;
}

std::vector<PlayerController*>& World::getPlayerControllers() {
	// TODO
	// TEMP, pendiente una modificacion, pero con la arq cliente-servidor esto probablemente ni exista
	pControllers = std::vector<PlayerController*>();

	for (std::vector<PlayerController*>& controllers : playerControllers) {
		pControllers.insert(pControllers.end(), controllers.begin(), controllers.end());
	}
	
	return pControllers;
}

void World::update(double dt)
{
	for (auto controllers: playerControllers) {
		for (auto player: controllers) {
			player->update(dt, this->getWidth(), this->getHeight(), ball.getModel().getX(),ball.getModel().getY());
			if (player->getModel()->getIsControlledByHuman()) {
				std::stringstream msg;
				msg << "Jugador Controlado: esta en " << player->getModel()->getX() << ", " << player->getModel()->getY();
				//std::cout << msg.str() << std::endl;
			}
		}
	}
	ball.update(dt, this->getWidth(), this->getHeight(), this->getPlayerControllers());
	//calculateCollision();
	updateBallController();
	std::stringstream msg;
	msg << "Pelota esta en " << ball.getModel().getX() << ", " << ball.getModel().getY();
	//std::cout << msg.str() << std::endl;
}

void World::serialize(model_data_t & modelData)
{
	for (auto controllers : playerControllers) {
		for (auto player : controllers) {
			player_view_data_t playerData;
			player->serialize(playerData);
			modelData.playerViewData.push_back(playerData);
		}
	}

	ball.serialize(modelData.ballViewData);
}

int World::getWidth(){
    return width;
}

int World::getHeight(){
    return height;
}

void World::swapToBallController(PlayerController *cont) {
    Team team = cont->getModel()->getTeam();
    std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(team)];
    for(PlayerController* controller : teamControllers){
        if(controller->getModel()->getHasControlOfTheBall()){
            cont->swap(controller);
        }
    }


}

void World::updateBallController() {
	PlayerController* priorController = NULL;
	PlayerController* currentController = NULL;
    int i=0;
	for (auto controllers : playerControllers) {
		for (auto player : controllers) {
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
                controlCounter=0;
            }
            i++;
		}

	}
	//std::cout<<"ENTRE"<<priorController->getModel()->getTeam()<<currentController->getModel()->getTeam()<<std::endl;

	if (priorController != NULL && currentController != NULL && priorController->getModel()->getTeam() == currentController->getModel()->getTeam()) {
		//std::cout<<"ENTRE"<<priorController->getModel()->getTeam()<<currentController->getModel()->getTeam()<<std::endl;
		priorController->swap(currentController);
		controlCounter=0;
	}


}

void World::calculateCollision() {
	int x = ball.getModel().getX();
	int y = ball.getModel().getX();
	int i = 0;
	std::vector<PlayerController *> &playerControllers = this->getPlayerControllers();
	for (PlayerController* controller : playerControllers) {
		int xPlayer = controller->getModel()->getCenterX();
		int yPlayer = controller->getModel()->getCenterY();
		if (abs(x - xPlayer)<10 && abs(y - yPlayer)<10) {
			if (!controller->getModel()->getHasControlOfTheBall()) {
				controller->getModel()->setHasControlOfTheBall(true);
				//controller->getModel()->setAngle(-90+ballModel.getAngle());
				changeController(i, playerControllers);
			}
		}
		i++;
	}
}

void World::changeController(int newController, std::vector<PlayerController *> &playerControllers) {
	int counter = 0;
	for (PlayerController* controller : playerControllers) {
		if (newController == counter) {
			controller->getModel()->setHasControlOfTheBall(true);
			ball.getModel().setVelX(controller->getModel()->getVelX());
			ball.getModel().setVelY(controller->getModel()->getVelY());
			//std::cout<<"Agarro la pelota"<< "Ball VelX: "<<ballModel.getVelX()<<" Ball VelY: "<<ballModel.getVelY()<<std::endl;
		}
		else {
			controller->getModel()->setHasControlOfTheBall(false);
		}
		counter++;
	}

}