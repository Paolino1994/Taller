#pragma once
class System
{
public:
	System() {};
	virtual ~System() {};

	virtual void process(double dt) = 0;
};

