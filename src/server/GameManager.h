#pragma once
#include <thread>
#include <iostream>

#include "common/EventHandler.h"
#include "common/GameConstants.h"
#include "UserManager.h"

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

	User_ID lastBallControlUser;

	int goalsByUser[4] = {0,0,0,0};

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

	static FIELD_POSITION getKickOffSideAfterPeriodEnd();

	static Team getKickOffTeamAfterGoal(GoalEvent& e);

	FIELD_POSITION getKickOffSideAfterGoal(GoalEvent& e);

	FIELD_POSITION getHomeDefends();

	FIELD_POSITION getAwayDefends();

	FIELD_POSITION getDefendedGoal(Team team);

	User_ID getLastBallControlUser();

	void setLastBallControlUser(User_ID userId);

	void goalScoredByUser();

	void switchTeamFieldPositions();

	// Events to handle:
	virtual void handle(KickEvent& e);
	virtual void handle(GoalEvent& e);
	virtual void handle(PeriodEndEvent& e);
	virtual void handle(GoalKickEvent& e);
	virtual void handle(PeriodStartEvent& e);
};
