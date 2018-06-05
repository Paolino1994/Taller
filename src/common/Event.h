#pragma once
#include <cstdint>

#include "GameConstants.h"

// no hagan includes aca, solo forward declarations (asi no hay problemas de dependencias circulares)
class EventHandler;
class PlayerModel;

// EventID definirlos en GameConstants!

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
	const PlayerModel& player; // el que pateo

	KickEvent(PlayerModel& player);
	virtual ~KickEvent();

	virtual EventID getId() {
		return EventID::KICK;
	}

	virtual void accept(EventHandler& handler);
};

class PeriodEndEvent : public Event {
public:
	const Team teamToKickOffNextPeriod;

	PeriodEndEvent(Team teamToKickOffNextPeriod);
	virtual ~PeriodEndEvent();

	virtual EventID getId() {
		return EventID::PERIOD_END;
	}

	virtual void accept(EventHandler& handler);
};