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

class GoalEvent : public Event {
public:
	const PlayerModel* scorer; // el que metio el gol, 
	const Team team; // para que equipo cuenta
	//cosnt Goal goal; // en que arco se metio! // ver

	GoalEvent(const PlayerModel* scorer, Team team);
	virtual ~GoalEvent();

	virtual EventID getId() {
		return EventID::GOAL;
	}

	virtual void accept(EventHandler& handler);
};

class GoalKickEvent : public Event {
public:
	const Team team; // que equipo saca
	const int goal;

	GoalKickEvent(Team team, int goal);
	virtual ~GoalKickEvent();

	virtual EventID getId() {
		return EventID::GOAL_KICK;
	}

	virtual void accept(EventHandler& handler);
};

class PostHitEvent : public Event {
public:
	PostHitEvent();
	virtual ~PostHitEvent();

	virtual EventID getId() {
		return EventID::POSTHIT;
	}

	virtual void accept(EventHandler& handler);
};


