#pragma once

#include <string>

#include "SDL.h"
#include "SoundManager.h"
#include "common/Protocol.h"
#include "common/GameConstants.h"
#include "common/Event.h"

// Mas que CommandSender, un RequestIssuer o otra zaraza
class CommandSender
{
private:
	Protocol protocol;
	std::vector<player_view_data_t> playerViewData;
	ball_view_data_t ballViewData;
	game_manager_data_t gameManagerData;
	std::vector<EventID> events;

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
	std::vector<EventID>& getEvents();
	int set_rcv_timeout(time_t seconds);

};

