#include "GameManager.h"

GameManager::GameManager():
	scoreHome(0),
	scoreAway(0),
	teamBallControl(Team::__LENGTH__),
	gameTimeInSeconds(0),
	ballInPlay(false)
{
	this->registerTo(EventID::KICK);
	this->registerTo(EventID::GOAL);
	this->registerTo(EventID::PERIOD_END);
}

GameManager& GameManager::get_instance() {
	static GameManager instance;
	return instance;
}

void GameManager::setTeamBallControl(Team team) {
    teamBallControl = team;
}

Team GameManager::getTeamBallControl() {
    return teamBallControl;
}

void GameManager::setGameTime(double newGameTimeInSeconds)
{
	this->gameTimeInSeconds = newGameTimeInSeconds;
}

bool GameManager::isBallInPlay()
{
	return this->ballInPlay;
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
	game_manager_data.timeInSeconds = this->gameTimeInSeconds;
}

Team GameManager::getKickOffTeamAfterGoal(GoalEvent & e)
{
	if (e.team == Team::HOME) {
		return Team::AWAY;
	}
	return Team::HOME;
}

void GameManager::handle(KickEvent & e)
{
	// Un kick cuando la pelota no esta en juego pasa cuando:
	// se saca del medio, o saca el arquero
	if (!this->ballInPlay) {
		this->ballInPlay = true;
	}
}

void GameManager::handle(GoalEvent & e)
{
	this->addGoal(e.team);
	this->ballInPlay = false; // esperamos el kickOff
}

void GameManager::handle(PeriodEndEvent & e)
{
	this->ballInPlay = false; // esperamos el kickOff
}


GameManager::~GameManager() {
}
