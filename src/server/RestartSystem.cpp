#include "RestartSystem.h"

#include "GameManager.h"
#include "common/EventQueue.h"

RestartSystem::RestartSystem(size_t secondsToWaitForRestart):
	secondsToWaitForRestart(secondsToWaitForRestart),
	secondsRemaining(secondsToWaitForRestart),
	counting(false)
{
	this->registerTo(EventID::GAME_END);
}

RestartSystem::~RestartSystem()
{
	this->unregisterFrom(EventID::GAME_END);
}

void RestartSystem::process(double dt)
{
	if (this->counting) {
		this->secondsRemaining -= dt;

		if (this->secondsRemaining < 0.0) {
			EventQueue::get().push(std::make_shared<GameRestartEvent>(Team::HOME));
			this->counting = false;
			this->secondsRemaining = 0.0;
		}

		GameManager::get_instance().setRestartCountdownRemainingSeconds(this->secondsRemaining);
	}
}

void RestartSystem::handle(GameEndEvent & e)
{
	this->secondsRemaining = this->secondsToWaitForRestart;
	this->counting = true;
}

