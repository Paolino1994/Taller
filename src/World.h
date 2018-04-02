#pragma once

#include <vector>

#include "Texture.h"
#include "Entity.h"

//Por ahora esto solo es un juntadero de objetos del juego:
class World
{
private:
    // El fondo (la cancha)
    Texture* background;
    // Objetos comunes
    std::vector<Entity*> entities;
    // Las dimensiones del mapa en screen coordinates
	// en x: vamos de 0 -> width
	// en y: vamos de 0 -> height
	// misma logica que la de renderizacion (x aumenta a derecha, y aumenta para abajo)
    int width;
    int height;
public:
    World(int width, int height, Texture* background);
    ~World();
    void addEntity(Entity* entity);
    
    Texture* getBackground() ;
    std::vector<Entity*>& getEntities() ;
    
    int getWidth();
    int getHeight();
};

