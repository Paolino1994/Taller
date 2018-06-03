#pragma once

struct Command;
class PlayerController;

class PlayerControllerScriptedState
{
protected:
	bool finished;
public:
	PlayerControllerScriptedState() : finished(false) {};
	virtual ~PlayerControllerScriptedState() {};

	virtual void handleEvent(Command &e) = 0;

	virtual void update(double dt, int x_limit, int y_limit, int ball_x, int ball_y) = 0;

	virtual bool isFinished() { return finished; };

	virtual PlayerControllerScriptedState* getNextState() = 0;
};

