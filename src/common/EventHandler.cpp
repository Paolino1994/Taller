#include "EventHandler.h"

#include "EventQueue.h"


EventHandler::EventHandler()
{
}


EventHandler::~EventHandler()
{
}

bool EventHandler::registerTo(EventID eventId)
{
	return EventQueue::get().registerTo(eventId, this);
}

bool EventHandler::unregisterFrom(EventID eventId) {
	return EventQueue::get().unRegisterFrom(eventId, this);
}