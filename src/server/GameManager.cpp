#include "GameManager.h"

#include "common/EventQueue.h"

GameManager::GameManager():
	scoreHome(0),
	scoreAway(0),
	teamBallControl(Team::__LENGTH__),
	gameTimeInSeconds(0),
	period(0),
	homeDefends(FIELD_POSITION::LEFT),
	awayDefends(FIELD_POSITION::RIGHT),
	ballInPlay(false),
	lastBallControlUser(-1)
{
	this->registerTo(EventID::KICK);
	this->registerTo(EventID::GOAL);
	this->registerTo(EventID::PERIOD_END);
	this->registerTo(EventID::GOAL_KICK);
	this->registerTo(EventID::PERIOD_START);
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
	game_manager_data.period = this->period;
}

FIELD_POSITION GameManager::getKickOffSideAfterPeriodEnd()
{
	return FIELD_POSITION::LEFT;
}

Team GameManager::getKickOffTeamAfterGoal(GoalEvent & e)
{
	if (e.team == Team::HOME) {
		return Team::AWAY;
	}
	return Team::HOME;
}

FIELD_POSITION GameManager::getKickOffSideAfterGoal(GoalEvent & e)
{
	if (e.team == Team::HOME) {
		return awayDefends;;
	}
	return homeDefends;
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

void GameManager::handle(GoalKickEvent & e)
{
	this->ballInPlay = false; // esperamos el kickOff
}

void GameManager::handle(PeriodStartEvent & e)
{
	this->ballInPlay = true;
	this->period++;
	if (this->period > 2) {
		this->period = 1;
	}
}

FIELD_POSITION GameManager::getHomeDefends() {
	return homeDefends;
}

FIELD_POSITION GameManager::getAwayDefends() {
	return awayDefends;
}

FIELD_POSITION GameManager::getDefendedGoal(Team team) {
	return team == Team::HOME ? homeDefends : awayDefends;
}

User_ID GameManager::getLastBallControlUser(){
	return lastBallControlUser;
}

void GameManager::setLastBallControlUser(User_ID userId){
	lastBallControlUser = userId;
}

void GameManager::goalScored(FIELD_POSITION fieldPostion){
	goalsByUser[lastBallControlUser - 1] ++;
	UserManager::get_instance().addGoal(lastBallControlUser);

	for(int i = 0 ; i < 4; i++) {
		std::cout << "User: " << i + 1 << " Goles: " << goalsByUser[i] << std::endl;
	}

}

void GameManager::switchTeamFieldPositions()
{
	FIELD_POSITION temp = homeDefends;
	homeDefends = awayDefends;
	awayDefends = temp;
}


GameManager::~GameManager() {
}
