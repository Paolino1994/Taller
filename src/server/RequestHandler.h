#pragma once

#include <thread>

#include "common/Socket.h"
#include "common/Protocol.h"

class RequestHandler
{
private:
	Protocol protocol;
	bool running;
	bool server_exit_requested;
	std::thread worker;

	void _run();

public:
	RequestHandler(Socket* socket);
	~RequestHandler();
	void run();
};


