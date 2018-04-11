#pragma once

#include "Entity.h"
#include "Player.h"
#include "World.h"

class Camera
{
	World& world;
    int width;
    int height;
    int x; //real world x, using 2d screen coordinates
    int y; //real world y , idem
    int x_update_offset; // el offset con respecto al estado anterior
    int y_update_offset; // idem
    Entity* followed; //std::vector<GameObject*> o Entities
    
public:
    Camera(World& world, int width, int height);
    ~Camera();
    
    void follow(Entity* gameObj); //Entity
    
    void update(double dt);
    
    void render(World& world);

};

