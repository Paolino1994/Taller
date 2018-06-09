#pragma once
#include "Texture.h"
#include "Score.h"
#include "MiniMap.h"

class InfoPanel
{
private:
	int width;
	int height;
	Texture& backgroundPanelRect;
	Score& score;
	MiniMap& miniMap;
	const int miniMap_x;
	const int miniMap_y;
public:
	InfoPanel(int width, int height, Texture& backgroundPanelRect, Score& score, MiniMap& miniMap);
	~InfoPanel();

	void update(double dt);
	void render(World& world, Camera& camera, int screen_x, int screen_y);
};

