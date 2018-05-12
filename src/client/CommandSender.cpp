#include "CommandSender.h"

#include "common/Request.h"


CommandSender::CommandSender(std::string ip, unsigned short port):
	protocol(Protocol(ip, port))
{
}


CommandSender::~CommandSender()
{
	protocol.shutdown();
}

void CommandSender::handleEvent(SDL_Event& e)
{
	Request request = Request::NONE;
	Command command = Command::__LENGTH__;

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		request = Request::COMMAND_KEY_DOWN;
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) 
	{
		request = Request::COMMAND_KEY_UP;
	}
		
	if (request != Request::NONE){
		switch (e.key.keysym.sym)
		{
			case SDLK_UP: {
				command = Command::GO_UP;
				break;
			}
			case SDLK_DOWN: {
				command = Command::GO_DOWN;
				break;
			}
			case SDLK_LEFT: {
				command = Command::GO_LEFT;
				break;
			}
			case SDLK_RIGHT: {
				command = Command::GO_RIGHT;
				break;
			}
			case SDLK_a: {
				command = Command::SWEEP;
				break;
			}
			case SDLK_SPACE: {
				command = Command::SPRINT;
				break;
			}
			case SDLK_s: {
				command = Command::KICK;
				break;
			}
			case SDLK_q: {
				command = Command::CHANGE_PLAYER;
				break;
			}
		}

		if (command != Command::__LENGTH__) {
			protocol.write(request, reinterpret_cast<const char*>(&command), sizeof(command));
		}
	}
}

short CommandSender::login(std::string credentials) {
	protocol.write(Request::LOGIN, credentials.c_str(), credentials.length());
	protocol.read();
	if (protocol.request() == Request::LOGIN) {
		int result = std::stoi(protocol.dataBuffer());
		if (result == USER_ACCEPTED) {
			return LOGIN_SUCCESS;
		} else {
			return result;
		}
	} else {
		return LOGIN_ERROR;
	}
}
