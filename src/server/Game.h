#pragma once
#include <thread>

#include "World.h"

class Game
{
private:
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

	PlayerController* assignToTeam(Team team);
};

