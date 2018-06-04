#pragma once
#include <unordered_set>
#include <queue>
#include <memory>

#include "EventHandler.h"
#include "Event.h"

class EventQueue
{
private:
	std::vector<std::unordered_set<EventHandler*>> eventHandlers;
	std::queue<std::shared_ptr<Event>> events; // == deque

	EventQueue();
	~EventQueue();
public:
	// C++11 singleton
	static EventQueue& get();
	EventQueue(EventQueue const&) = delete;
	void operator=(EventQueue const&) = delete;

	bool registerTo(EventID eventId, EventHandler* handler); //weakPtr ?
	bool unRegisterFrom(EventID eventId, EventHandler * handler);
	void push(std::shared_ptr<Event> event);
	void handleEvents();
	void getEvents(std::queue<std::shared_ptr<Event>>& queueToFill);
};

