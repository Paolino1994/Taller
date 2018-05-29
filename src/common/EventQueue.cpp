#include "EventQueue.h"



EventQueue & EventQueue::get()
{
	static EventQueue instance;
	return instance;
}

EventQueue::EventQueue():
	eventHandlers(std::vector<std::vector<EventHandler*>>(static_cast<std::underlying_type<EventID>::type>(EventID::__LENGTH__))),
	events(std::queue<std::shared_ptr<Event>>())
{
}


EventQueue::~EventQueue()
{
}

bool EventQueue::registerTo(EventID eventId, EventHandler * handler)
{
	size_t event_id = static_cast<std::underlying_type<EventID>::type>(eventId);
	if (event_id < eventHandlers.size()) {
		eventHandlers[event_id].push_back(handler);
		return true;
	}
	return false;
}

void EventQueue::push(std::shared_ptr<Event> event)
{
	this->events.push(event);
}

void EventQueue::handleEvents()
{
	while (!events.empty()) {
		std::shared_ptr<Event> event = events.front();
		size_t event_id = static_cast<std::underlying_type<EventID>::type>(event->getId());
		for (EventHandler* handler : eventHandlers[event_id]) {
			event->accept(*handler);
		}
		events.pop();
	}
}

void EventQueue::getEvents(std::queue<std::shared_ptr<Event>>& queueToFill)
{
	queueToFill = std::queue<std::shared_ptr<Event>>(this->events);
}
