#pragma once

#include "Entity.h"
//#include "Player.h"
#include "World.h"
#include "Texture.h"

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
    Texture *miniMapRect;
    Texture *miniFieldRect;
    
public:
    Camera(World& world, int width, int height, int widthScrollOffset, int heightScrollOffset, Texture *miniMapRect, Texture *miniFieldRect);
    ~Camera();
    
    void follow(Entity& gameObj); //Entity

	bool isWithinScrollBoundaries(Entity* entity);
    
    void update(double dt);
    
    void render(World& world);

};

