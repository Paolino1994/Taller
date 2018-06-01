#pragma once
#include <thread>

#include "common/EventHandler.h"
#include "World.h"
#include "common/GameConstants.h"

class GameManager
{
private:

	static bool initialized;
    static GameManager* instance;
    int scoreHome;
    int scoreAway;
    Team teamBallControl;
	GameManager();

public:
	virtual ~GameManager();

    static void initialize();

    static bool is_initialized();

    static GameManager* get_instance(); 

    void addGoal(Team team);

    void setTeamBallControl(Team team);

    Team getTeamBallControl();

    void serialize(game_manager_data_t& game_manager_data);
};
