#pragma once
#include <thread>
#include <mutex>

#include "World.h"

class Game
{
private:
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

	std::mutex updateMtx;
	std::mutex serializeMtx;
	std::mutex teamFormationMtx;

	void _run();

public:
	Game();
	virtual ~Game();

	PlayerController* assignToTeam(Team team, User_ID userId);
	bool checkTeamFormation(Team team, User_ID userId);
	void setTeamFormation(Team team, Formation formation, User_ID userId);

	size_t getModelSnapshotNumber();
	void copyModelData(model_data_t& modelDataReceiver);

	// invalida el contoller, le hacemos delete
	bool withdrawUser(PlayerController* playerController, User_ID userId);

	void handleCommandForPlayer(PlayerController* player, Command& command);
};

