#pragma once

#include <thread>

#include "common/Socket.h"
#include "common/Protocol.h"
#include "Game.h"

class RequestHandler
{
private:
	Protocol protocol;
	Game& game;
	bool running;
	bool server_exit_requested;
	std::thread worker;

	// De aca para abajo quizas manejar con state en un futuro
	// Ya que en un principio, va a trabajar sobre el inicio de sesion con el Game
	// y recien cuando este ok, este player pasa de NULL a algo valido!;
	PlayerController* player;

	void _run();

public:
	RequestHandler(Socket* socket, Game& game);
	~RequestHandler();
	void run();
};


