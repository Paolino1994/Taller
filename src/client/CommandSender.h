#pragma once

#include <string>

#include "SDL.h"
#include "common/Protocol.h"
#include "common/GameConstants.h"

// Mas que nada para testing esta esto, pero se puede convertir en el posta
class CommandSender
{
private:
	
public:
	Protocol protocol; // TEMP
	CommandSender(std::string ip, unsigned short port, Team team);
	~CommandSender();
	void handleEvent(SDL_Event& e);
};

