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

Texture* World::getPlayerSelectedTexture() {
    return playerSelectedTexture;
}

void World::setPlayerSelectedTexture(Texture* texture) {
    playerSelectedTexture = texture;
}

std::vector<Entity*>& World::getEntities() {
    return entities;
}

std::vector<PlayerController*>& World::getPlayerControllers() {
    return pControllers;
}

void World::update(double dt)
{
	for (auto player : pControllers) {
		player->update(dt, this->getWidth(), this->getHeight());
	}
}

int World::getWidth(){
    return width;
}

int World::getHeight(){
    return height;
}
