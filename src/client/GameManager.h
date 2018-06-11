#pragma once
#include <thread>

#include "World.h"

class GameManager
{
private:

	static bool initialized;
    static GameManager* instance;

    int scoreHome;
    int scoreAway;
	int time;

    int period;

	int goalsByUser[4] = {0,0,0,0};

    double displayGoalText;

    double displayGoalKickText;

	bool showGoalStatistics;

	int restartCountdownSecondsRemaining;

	GameManager();

public:
	virtual ~GameManager();

    static void initialize();

    static bool is_initialized();

    static GameManager* get_instance(); 

    void update(const game_manager_data_t& game_manager_data);

    int getScore(Team team);

	int getTime();

	int getPeriod();

    int getGoalsByUser(User_ID userId);

    void setDisplayGoalText(double value);

	double getDisplayGoalText();

    void setDisplayGoalKickText(double value);

	double getDisplayGoalKickText();

	bool showGoalStats();

	void setShowGoalStats(bool show);

	int getRestartCountdownSecondsRemaining();
};
