#pragma once
class System
{
public:
	System() {};
	virtual ~System() {};

	virtual void process(double dt) = 0; // quizas el tiempo no deberia estar, pero lo dejamos igual
};

