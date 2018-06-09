#pragma once

#include "Entity.h"
#include "World.h"

class Camera
{
	World& world;
    const int width;
    const int height;
	const int widthScrollOffset;
	const int heightScrollOffset;
    int x; //real world x, using 2d screen coordinates
    int y; //real world y , idem
    int x_update_offset; // el offset con respecto al estado anterior
    int y_update_offset; // idem
    Entity& followed; //std::vector<GameObject*> o Entities    
public:
    Camera(World& world, int width, int height, int widthScrollOffset, int heightScrollOffset);
    ~Camera();
    
    void follow(Entity& gameObj); //Entity

	bool isWithinScrollBoundaries(Entity* entity);
    
    void update(double dt);
    
    void render(World& world, int screen_x, int screen_y);

	int getX();
	int getY();

	int getWidth();
	int getHeight();
};

