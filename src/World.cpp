#include "World.h"

World::World(int width, int height, Texture* background):
    background(background),
    entities(std::vector<Entity*>()),
    width(width),
    height(height)
{
}

World::~World()
{
}

void World::addEntity(Entity* entity){
    entities.push_back(entity);
}

Texture* World::getBackground() {
    return background;
}

std::vector<Entity*>& World::getEntities() {
    return entities;
}

int World::getWidth(){
    return width;
}

int World::getHeight(){
    return height;
}