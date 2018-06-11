#include "GameManager.h"
#include "../common/GameConstants.h"

GameManager* GameManager::instance = 0;
bool GameManager::initialized = false;

GameManager::GameManager(){
    scoreHome = 0;
    scoreAway = 0;
	time = 0;
	period = 0;
    displayGoalText = 0;
    displayGoalKickText = 0;
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
    time = game_manager_data.timeInSeconds;
    period = game_manager_data.period;
    goalsByUser[0] = game_manager_data.user1Goals;
    goalsByUser[1] = game_manager_data.user2Goals;
    goalsByUser[2] = game_manager_data.user3Goals;
    goalsByUser[3] = game_manager_data.user4Goals;
    
}

int GameManager::getScore(Team team){
    if (team == Team::HOME) {
        return scoreHome;
    } else {
        return scoreAway;
    }
}

int GameManager::getGoalsByUser(User_ID userId){
    return goalsByUser[userId - 1];
}

double GameManager::getDisplayGoalText() {
    return displayGoalText;
}

void GameManager::setDisplayGoalText(double value) {
    displayGoalText = value;
}

double GameManager::getDisplayGoalKickText() {
    return displayGoalKickText;
}

void GameManager::setDisplayGoalKickText(double value) {
    displayGoalKickText = value;
}


GameManager::~GameManager() {
}

int GameManager::getTime() {
    return time;
}

int GameManager::getPeriod()
{
	return this->period;
}


