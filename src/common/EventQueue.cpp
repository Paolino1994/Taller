#include "EventQueue.h"



EventQueue & EventQueue::get()
{
	static EventQueue instance;
	return instance;
}

EventQueue::EventQueue():
	eventHandlers(std::vector<std::unordered_set<EventHandler*>>(static_cast<std::underlying_type<EventID>::type>(EventID::__LENGTH__))),
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
		eventHandlers[event_id].insert(handler);
		return true;
	}
	return false;
}

bool EventQueue::unRegisterFrom(EventID eventId, EventHandler * handler)
{
	size_t event_id = static_cast<std::underlying_type<EventID>::type>(eventId);
	if (event_id < eventHandlers.size()) {
		if (eventHandlers[event_id].erase(handler) > 0) {
			return true;
		}
		return false;
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
		auto handlers_copy = std::unordered_set<EventHandler*>(eventHandlers[event_id]); // por si se desregistran mientras handlean el evento
		for (EventHandler* handler : handlers_copy) {
			event->accept(*handler);
		}
		events.pop();
	}
}

void EventQueue::getEvents(std::queue<std::shared_ptr<Event>>& queueToFill)
{
	queueToFill = std::queue<std::shared_ptr<Event>>(this->events);
}
