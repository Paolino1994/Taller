#include "GameManager.h"

GameManager::GameManager():
	scoreHome(0),
	scoreAway(0),
	teamBallControl(Team::__LENGTH__),
	gameTimeInSeconds(0),
	ballInPlay(false),
	period(1),
	homeDefends(FIELD_POSITION::LEFT),
	awayDefends(FIELD_POSITION::RIGHT)
{}

GameManager& GameManager::get_instance() {
	static GameManager instance;
	instance.registerTo(EventID::KICK);
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
	game_manager_data.homeDefends = homeDefends;
	game_manager_data.awayDefends = awayDefends;
}

void GameManager::handle(KickEvent & e)
{
	// Un kick cuando la pelota no esta en juego pasa cuando:
	// se saca del medio, o saca el arquero
	if (!this->ballInPlay) {
		this->ballInPlay = true;
	}
}


GameManager::~GameManager() {
}
