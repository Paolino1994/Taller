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

	GameManager();

public:
	virtual ~GameManager();

    static void initialize();

    static bool is_initialized();

    static GameManager* get_instance(); 

    void update(const game_manager_data_t& game_manager_data);

    int getScore(Team team);
};
