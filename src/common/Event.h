#pragma once
#include <cstdint>

class EventHandler;

enum class EventID : uint32_t {
	KICK = 0,
	__LENGTH__
};

class Event
{
public:
	Event();
	virtual ~Event();

	virtual EventID getId() = 0;
	virtual void accept(EventHandler& handler) = 0;  //ES NECESARIO Redefinir este para cada evento especifico, a pesar de que el codigo sea identico
};

class KickEvent : public Event {
public:
	KickEvent();
	virtual ~KickEvent();

	virtual EventID getId() {
		return EventID::KICK;
	}

	virtual void accept(EventHandler& handler);
};