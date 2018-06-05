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
