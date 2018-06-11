#pragma once
#include <cstddef>

#include "System.h"
#include "common/EventHandler.h"

class RestartSystem: public System, public EventHandler
{
private:
	const size_t secondsToWaitForRestart;
	double secondsRemaining;
	bool counting;
public:
	RestartSystem(size_t secondsToWaitForRestart);
	~RestartSystem();

	virtual void process(double dt);

	// Events to handle
	virtual void handle(GameEndEvent& e);
};

