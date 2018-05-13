#pragma once
#include <thread>

#include "World.h"

class Game
{
private:
	std::vector<player_view_data_t> playerViewData;
	ball_view_data_t ballViewData;
	model_data_t modelData;

	World world;
	const size_t maxPlayers;
	size_t playerCount;

	bool running;
	bool server_exit_requested;
	std::thread worker;

	void _run();

public:
	Game();
	~Game();

	PlayerController* assignToTeam(Team team, User_ID userId);
	model_data_t getModelData();
};

