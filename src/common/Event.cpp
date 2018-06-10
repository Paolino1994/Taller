#include "Event.h"
#include "EventHandler.h"


Event::Event()
{}

Event::~Event()
{}

KickEvent::KickEvent(PlayerModel & player):
	player(player)
{}

KickEvent::~KickEvent()
{
}

void KickEvent::accept(EventHandler & handler)
{
	handler.handle(*this);
}

PeriodEndEvent::PeriodEndEvent(Team teamToKickOffNextPeriod):
	teamToKickOffNextPeriod(teamToKickOffNextPeriod)
{}

PeriodEndEvent::~PeriodEndEvent()
{
}

void PeriodEndEvent::accept(EventHandler & handler)
{
	handler.handle(*this);
}

GoalEvent::GoalEvent(const PlayerModel * scorer, Team team):
	scorer(scorer),
	team(team)
{}

GoalEvent::~GoalEvent()
{}

void GoalEvent::accept(EventHandler & handler)
{
	handler.handle(*this);
}

// GOAL KICK EVENT
GoalKickEvent::GoalKickEvent(Team team, int goal):
	team(team),
	goal(goal)
{}

GoalKickEvent::~GoalKickEvent()
{}

void GoalKickEvent::accept(EventHandler & handler)
{
	handler.handle(*this);
}

PostHitEvent::PostHitEvent()
{}

PostHitEvent::~PostHitEvent()
{}

void PostHitEvent::accept(EventHandler & handler)
{
	handler.handle(*this);
}

PeriodStartEvent::PeriodStartEvent(Team teamThatKickedOff):
	teamThatKickedOff(teamThatKickedOff)
{
}

PeriodStartEvent::~PeriodStartEvent()
{
}

void PeriodStartEvent::accept(EventHandler & handler)
{
	handler.handle(*this);
}
