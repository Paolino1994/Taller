#include "World.h"
#include "BallController.h"
#include "TeamFactory.h"
#include "PlayerControllerHuman.h"
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

PlayerController* World::injectHumanController(Team team)
{
	std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(team)];

	for (size_t i = 0; i < teamControllers.size(); i++)
	{
		if (teamControllers[i]->isControllable()) {
			PlayerController* other = teamControllers[i];
			PlayerControllerHuman* human = new PlayerControllerHuman(other->getModel(), other->getView(), *this);
			teamControllers[i] = human;
			human->getModel()->setIsControlledByHuman(true);
            human->getModel()->setHasControlOfTheBall(true);
			delete other;
			return human;
		}
	}

	return nullptr;
}

bool World::playerIsOnRange(PlayerController* cont,PlayerController* controllerToSwap){
	PlayerModel* model=cont->getModel();
	int x=model->getX();
	int y=model->getY();
	int xCon=controllerToSwap->getModel()->getX();
	int yCon=controllerToSwap->getModel()->getY();
	if((x<xCon+200 || x>xCon-200) && (y<yCon+150 || y>yCon-150)){
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

    //size_t controllerToSwapIndex = index;

    do
    { // en el peor caso volvemos a el que estabamos controlando recien
        ++index;
        if (index == (long long)teamControllers.size()) {
            index = 0;
        }

    } while (teamControllers[index] != controllerToSwap && !teamControllers[index]->isControllable() && playerIsOnRange(teamControllers[index],controllerToSwap)); // sin chequeos de camara por ahora -> igual se sacaba para la fase 2
    return teamControllers[index];

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

	do
	{ // en el peor caso volvemos a el que estabamos controlando recien
		++index;
		if (index == (long long)teamControllers.size()) {
			index = 0;
		}

	} while (teamControllers[index] != controllerToSwap && !teamControllers[index]->isControllable() && playerIsOnRange(teamControllers[index],controllerToSwap)); // sin chequeos de camara por ahora -> igual se sacaba para la fase 2

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
			player->update(dt, this->getWidth(), this->getHeight());
			if (player->getModel()->getIsControlledByHuman()) {
				std::stringstream msg;
				msg << "Jugador Controlado: esta en " << player->getModel()->getX() << ", " << player->getModel()->getY();
				std::cout << msg.str() << std::endl;
			}
		}
	}
	ball.update(dt, this->getWidth(), this->getHeight(), this->getPlayerControllers());
	std::stringstream msg;
	msg << "Pelota esta en " << ball.getModel().getX() << ", " << ball.getModel().getY();
	std::cout << msg.str() << std::endl;
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