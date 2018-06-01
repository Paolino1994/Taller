#include "GameManager.h"

GameManager* GameManager::instance = 0;
bool GameManager::initialized = false;

GameManager::GameManager(){
    scoreHome = 0;
    scoreAway = 0;
}

void GameManager::initialize() {
    if (instance == 0) {
        instance = new GameManager();
    } 
    initialized = true;
}

bool GameManager::is_initialized() {
	return initialized;
}

GameManager* GameManager::get_instance() {
	if (instance == 0) {
		instance = new GameManager();
	}
	return instance;
}

void GameManager::update(const game_manager_data_t & game_manager_data)
{
	scoreHome = game_manager_data.scoreHome;
	scoreAway = game_manager_data.scoreAway;
}

int GameManager::getScore(Team team){
    if (team == Team::HOME) {
        return scoreHome;
    } else {
        return scoreAway;
    }
}


GameManager::~GameManager() {
}


