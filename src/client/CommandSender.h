#pragma once

#include <string>

#include "SDL.h"
#include "common/Protocol.h"
#include "common/GameConstants.h"

// Mas que CommandSender, un RequestIssuer o otra zaraza
class CommandSender
{
private:
	std::vector<player_view_data_t> playerViewData;
	ball_view_data_t ballViewData;

	// Quizas proximamente datos "estadisticos" del modelo
	// int modelUpdateNumber
	// Date/Time modelUpdateTime
	// .....

public:
	
	CommandSender(std::string ip, unsigned short port);
	~CommandSender();
	void handleEvent(SDL_Event& e);
	short login(std::string credentials);
	Request listenStart();
	void assignTeam(Team team);
	bool updateModel();
	model_data_t getModelData();
	int set_rcv_timeout(time_t seconds);
	Protocol protocol;

};

