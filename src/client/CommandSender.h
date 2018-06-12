#pragma once

#include <string>
#include <mutex>

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
	model_data_t modelData;
	std::vector<EventID> events;

	std::mutex requestMtx;
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
	bool checkFormation(Team team);
	void setTeamFormation(Team team, Formation formation);
	bool updateModel();
	model_data_t& getModelData();
	std::vector<EventID>& getEvents();
	int set_rcv_timeout(time_t seconds);

};

