#pragma once
#include <thread>

#include "common/EventHandler.h"
#include "World.h"

class Game : public EventHandler
{
private:
	std::vector<player_view_data_t> playerViewData;
	ball_view_data_t ballViewData;
	game_manager_data_t gameManagerData;
	std::vector<EventID> events;
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
	virtual ~Game();

	PlayerController* assignToTeam(Team team, User_ID userId);
	model_data_t getModelData();
	// invalida el contoller, le hacemos delete
	bool withdrawUser(PlayerController* playerController, User_ID userId);

	virtual void handleFallback(Event & e);

	//virtual void handle(KickEvent& e);
};

