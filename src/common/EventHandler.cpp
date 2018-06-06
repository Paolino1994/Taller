#include "EventHandler.h"

#include "EventQueue.h"


EventHandler::EventHandler()
{
}


EventHandler::~EventHandler()
{
	// por las dudas me desregistro de todos los eventos!
	this->unRegisterFromAllEvents();
}

bool EventHandler::registerTo(EventID eventId)
{
	return EventQueue::get().registerTo(eventId, this);
}

bool EventHandler::unregisterFrom(EventID eventId) {
	return EventQueue::get().unRegisterFrom(eventId, this);
}

void EventHandler::registerToAllEvents()
{
	EventQueue::get().registerToAll(this);
}

void EventHandler::unRegisterFromAllEvents()
{
	EventQueue::get().unRegisterFromAll(this);
}
