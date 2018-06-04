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
	int goalBottomPost = 420;
	int goalTopPost = 620;
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
				// Aca estoy probando el GameManager, no es para nada definitivo que sea asi
				GameManager::get_instance().addGoal(Team::HOME); // Obvio que no es asi porque falta la logica del cambio de lado
			} else {
				std::cout << "PALO" << std::endl;
				ballModel.setVelX( -ballModel.getVelX() ); // Hago rebotar la pelota en el palo, le cambio el angulo en x
			}
		} else {
        	std::cout << "Salio por el costado izquierdo" << std::endl;
		}
    }
	
}
