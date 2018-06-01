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

void GameManager::setTeamBallControl(Team team) {
    teamBallControl = team;
}

Team GameManager::getTeamBallControl() {
    return teamBallControl;
}

void GameManager::addGoal(Team team){
    if (team == Team::HOME) {
        scoreHome++;
    } else {
        scoreAway++;
    }
}

void GameManager::serialize(game_manager_data_t& game_manager_data) {
	game_manager_data.scoreHome = scoreHome;
	game_manager_data.scoreAway = scoreAway;
}


GameManager::~GameManager() {
}
