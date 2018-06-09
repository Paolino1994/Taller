#include "MiniMap.h"


MiniMap::MiniMap(int width, int height, double worldScale, Texture * cameraRectangle, Texture * background):
	width(width),
	height(height),
	worldScale(worldScale),
	cameraRectangle(cameraRectangle),
	background(background)
{
	background->setScaling(this->width, this->height);
}

MiniMap::~MiniMap()
{
}

void MiniMap::render(World& world, Camera& camera, int screen_x, int screen_y)
{
	std::map<Player_ID, Player>& players = world.getPlayers();

	//background->setScaling(this->width, this->height);
	background->render(screen_x, screen_y);


	int pos_x = screen_x + camera.getX() * worldScale;
	int pos_y = screen_y + camera.getY() * worldScale;
	cameraRectangle->setScaling(camera.getWidth() * worldScale, camera.getHeight() * worldScale);
	cameraRectangle->render(pos_x, pos_y);

	for (std::pair<const Player_ID, Player>& pair : players)
	{
		Player& player = pair.second;
		player.renderMiniMap(screen_x + player.getX() * worldScale, screen_y + player.getY() * worldScale);
	}

	world.getBall().renderMiniMap(screen_x + world.getBall().getX() * worldScale, screen_y + world.getBall().getY() * worldScale);
}

int MiniMap::getWidth()
{
	return this->width;
}

int MiniMap::getHeight()
{
	return this->height;
}
