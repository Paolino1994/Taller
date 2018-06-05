#pragma once
#include <thread>

#include "common/EventHandler.h"
#include "World.h"
#include "common/GameConstants.h"

class GameManager: public EventHandler
{
private:
    int scoreHome;
    int scoreAway;
    Team teamBallControl;
	double gameTimeInSeconds;
	int period;
	FIELD_POSITION homeDefends;
	FIELD_POSITION awayDefends;
	bool ballInPlay; // pelota esta en juego o no

	GameManager();
	~GameManager();
public:
	// C++11 singleton
	static GameManager& get_instance();
	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

    void addGoal(Team team);

    void setTeamBallControl(Team team);

    Team getTeamBallControl();

	void setGameTime(double newGameTimeInSeconds);

	bool isBallInPlay();

    void serialize(game_manager_data_t& game_manager_data);

	static Team getKickOffTeamAfterGoal(GoalEvent& e);

	FIELD_POSITION getHomeDefends();

	FIELD_POSITION getAwayDefends();

	// Events to handle:
	virtual void handle(KickEvent& e);
	virtual void handle(GoalEvent& e);
	virtual void handle(PeriodEndEvent& e);
	virtual void handle(GoalKickEvent& e);


};
