#pragma once

#include <thread>
#include <chrono>
#include <vector>

#include "common/Socket.h"
#include "common/Protocol.h"
#include "common/EventHandler.h"
#include "Game.h"
#include "UserManager.h"

class RequestHandler: public EventHandler
{
private:
	Protocol protocol;
	Game& game;
	User_ID userId;
	bool running;
	bool server_exit_requested;
	std::thread worker;

	model_data_t modelData;
	size_t modelSnapshotNumber;
	std::vector<EventID> events;

	// De aca para abajo quizas manejar con state en un futuro
	// Ya que en un principio, va a trabajar sobre el inicio de sesion con el Game
	// y recien cuando este ok, este player pasa de NULL a algo valido!;
	PlayerController* player;

	void _run();

public:
	RequestHandler(Socket* socket, Game& game, User_ID userId);
	~RequestHandler();
	void run();

	// Events to handle
	virtual void handleFallback(Event& e);
};


