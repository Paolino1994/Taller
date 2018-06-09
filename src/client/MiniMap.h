#pragma once
#include "Texture.h"
#include "Camera.h"
#include "World.h"

class MiniMap
{
private:
	int width;
	int height;
	double worldScale;
	Texture* cameraRectangle;
	Texture* background;
public:
	MiniMap(int width, int height, double worldScale, Texture* cameraRectangle, Texture* background);
	~MiniMap();

	void render(World& world, Camera& camera, int screen_x, int screen_y);

	int getWidth();
	int getHeight();
};

