#include "TimerSystem.h"

#include "common/EventQueue.h"
#include "GameManager.h"
#include "PlayerModel.h"


Team TimerSystem::getNextKickOffTeam(Team previousTeam)
{
	switch (previousTeam) {
	case Team::HOME: {
		return Team::AWAY;
	}
	case Team::AWAY: {
		return Team::HOME;
	}
	default:
		return Team::HOME;
	}
}

TimerSystem::TimerSystem(size_t periodLengthInSeconds, bool emulateRegularGamePeriod) :
	periodLengthInSeconds(periodLengthInSeconds),
	timeMultiplier(emulateRegularGamePeriod ? (regularGamePeriodLengthInSeconds / (double)periodLengthInSeconds) : 1),
	periodSecondsElapsed(0),
	periodStarted(false),
	kickOffTeam(Team::__LENGTH__)
{
	this->registerTo(EventID::KICK);
}

TimerSystem::TimerSystem(size_t periodLengthInSeconds):
	TimerSystem(periodLengthInSeconds, true)
{}


TimerSystem::~TimerSystem()
{
}

void TimerSystem::process(double dt)
{
	if (this->periodStarted) {
		this->periodSecondsElapsed += dt;

		if (this->periodSecondsElapsed >= this->periodLengthInSeconds) {
			EventQueue::get().push(std::make_shared<PeriodEndEvent>(getNextKickOffTeam(this->kickOffTeam)));
			this->registerTo(EventID::KICK);
			this->periodStarted = false;
			this->periodSecondsElapsed = this->periodLengthInSeconds;
		}

		GameManager::get_instance().setGameTime(this->periodSecondsElapsed * timeMultiplier);
	}
}

void TimerSystem::handle(KickEvent & e)
{
	this->kickOffTeam = e.player.getTeam();
	this->periodStarted = true;
	this->periodSecondsElapsed = 0; // Ver si se quiere resetear el tiempo
	this->unregisterFrom(EventID::KICK);
	EventQueue::get().push(std::make_shared<PeriodStartEvent>(this->kickOffTeam));
}
