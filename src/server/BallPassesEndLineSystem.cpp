#include "BallPassesEndLineSystem.h"

#include <iostream>

BallPassesEndLineSystem::BallPassesEndLineSystem(World & world):
	world(world)
{}

BallPassesEndLineSystem::~BallPassesEndLineSystem()
{
}

void BallPassesEndLineSystem::process(double dt)
{
	BallModel& ballModel = world.getBall().getModel();

    int leftEndLineX = 60;
	int goalBottomPost = 400;
	int goalTopPost = 630;
	int postWidth = 30;
    //int rigthEndLineX = 30;

	int x = ballModel.getX();
	int y = ballModel.getY();
	// double z = ballModel.getZ();
	
	if( x < leftEndLineX )
    {
		if (y > goalBottomPost - postWidth && y < goalTopPost + postWidth)  // Tambien podemos poner la validacion en eje z para el travesanio
		{
			if(y > goalBottomPost && y < goalTopPost)
			{
				std::cout << "GOOOL" << std::endl;
			} else {
				std::cout << "PALO" << std::endl;
			}
		} else {
        	std::cout << "Salio por el costado izquierdo" << std::endl;
		}
    }
	
}
