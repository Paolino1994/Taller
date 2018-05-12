#include "CommandSender.h"

#include "common/Request.h"


CommandSender::CommandSender(std::string ip, unsigned short port, Team team):
	protocol(Protocol(ip, port))
{
	protocol.write(Request::TEAM_ASSIGN, reinterpret_cast<const char*>(&team), sizeof(team));
}


CommandSender::~CommandSender()
{
	protocol.shutdown();
}

void CommandSender::handleEvent(SDL_Event& e)
{
	Request request = Request::NONE;
	Command command = {CommandKey::__LENGTH__, CommandType::__LENGTH__};

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		request = Request::COMMAND;
		command.key = CommandKey::KEY_DOWN;
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) 
	{
		request = Request::COMMAND;
		command.key = CommandKey::KEY_UP;
	}
		
	if (request != Request::NONE){
		switch (e.key.keysym.sym)
		{
			case SDLK_UP: {
				command.type = CommandType::GO_UP;
				break;
			}
			case SDLK_DOWN: {
				command.type = CommandType::GO_DOWN;
				break;
			}
			case SDLK_LEFT: {
				command.type = CommandType::GO_LEFT;
				break;
			}
			case SDLK_RIGHT: {
				command.type = CommandType::GO_RIGHT;
				break;
			}
			case SDLK_a: {
				command.type = CommandType::SWEEP;
				break;
			}
			case SDLK_SPACE: {
				command.type = CommandType::SPRINT;
				break;
			}
			case SDLK_s: {
				command.type = CommandType::KICK;
				break;
			}
			case SDLK_q: {
				command.type = CommandType::CHANGE_PLAYER;
				break;
			}
		}

		if (command.key != CommandKey::__LENGTH__ && command.type != CommandType::__LENGTH__) {
			protocol.write(request, reinterpret_cast<const char*>(&command), sizeof(command));
		}
	}
}