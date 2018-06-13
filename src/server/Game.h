#pragma once
#include <thread>

#include "World.h"

class Game
{
private:
	std::vector<player_view_data_t> playerViewData;
	ball_view_data_t ballViewData;
	game_manager_data_t gameManagerData;
	model_data_t modelData;
	size_t modelSnapshotNumber;

	World world;
	const size_t maxPlayers;
	size_t playerCount;

	bool running;
	bool existFormationHome;
	bool existFormationAway;
	bool server_exit_requested;
	std::thread worker;

	void _run();

public:
	Game();
	virtual ~Game();

	PlayerController* assignToTeam(Team team, User_ID userId);
	bool checkTeamFormation(Team team, User_ID userId);
	void setTeamFormation(Team team, Formation formation, User_ID userId);
	model_data_t getModelData();

	size_t getModelSnapshotNumber();

	// invalida el contoller, le hacemos delete
	bool withdrawUser(PlayerController* playerController, User_ID userId);
};

