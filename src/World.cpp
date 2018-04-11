#include "World.h"

World::World(int width, int height, Texture* background):
    background(background),
    entities(std::vector<Entity*>()),
	pControllers(std::vector<PlayerController*>()),
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

void World::addPlayerController(PlayerController* pController){
	pControllers.push_back(pController);
}

Texture* World::getBackground() {
    return background;
}

std::vector<Entity*>& World::getEntities() {
    return entities;
}

std::vector<PlayerController*>& World::getPlayerControllers() {
    return pControllers;
}

int World::getWidth(){
    return width;
}

int World::getHeight(){
    return height;
}
