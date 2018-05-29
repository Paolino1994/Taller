#include "Event.h"
#include "EventHandler.h"


Event::Event()
{
}


Event::~Event()
{
}

KickEvent::KickEvent()
{
}

KickEvent::~KickEvent()
{
}

void KickEvent::accept(EventHandler & handler)
{
	handler.handle(*this);
}
