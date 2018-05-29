#include "EventHandler.h"

#include "EventQueue.h"


EventHandler::EventHandler()
{
}


EventHandler::~EventHandler()
{
}

void EventHandler::registerTo(EventID eventId)
{
	EventQueue::get().registerTo(eventId, this);
}
