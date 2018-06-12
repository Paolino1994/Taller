#pragma once
#include <cstddef>
#include "System.h"
#include "common/EventHandler.h"
#include "common/YAMLReader.h"


class TimerSystem: public System, public EventHandler
{
private:
	static const size_t regularGamePeriodLengthInSeconds = 45 * 60;
	const size_t periodLengthInSeconds;
	const double timeMultiplier;
	double periodSecondsElapsed;
	bool periodStarted;
	Team kickOffTeam;

	static Team getNextKickOffTeam(Team previousTeam);
public:
	/* pasar:
	** periodLengthInSeconds: tiempo que tarda un tiempo en el server (tiempo real)
	** emulateRegularGamePeriod: true, si queremos emular tiempo de juego de 45 min (regularGamePeriodLengthInSeconds)
	**							 o sea, el tiempo iria mas rapido
	*/
	TimerSystem(size_t periodLengthInSeconds, bool emulateRegularGamePeriod);
	TimerSystem(size_t periodLengthInSeconds); // con emulacion = true
	virtual ~TimerSystem();

	virtual void process(double dt);

	// Events to handle:
	virtual void handle(KickEvent& e);
};

