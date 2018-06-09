#include "InfoPanel.h"

#include <cmath>

InfoPanel::InfoPanel(int width, int height, Texture& backgroundPanelRect, Score& score, MiniMap& miniMap) :
	width(width),
	height(height),
	backgroundPanelRect(backgroundPanelRect),
	score(score),
	miniMap(miniMap),
	miniMap_x(width / 2 - miniMap.getWidth() / 2),
	miniMap_y(std::max(0, height / 2 - miniMap.getHeight() / 2))
{
	backgroundPanelRect.setScaling(width, height);
}


InfoPanel::~InfoPanel()
{
}

void InfoPanel::update(double dt)
{
	if (GameManager::get_instance()->getDisplayGoalText() > 0) {
		GameManager::get_instance()->setDisplayGoalText(GameManager::get_instance()->getDisplayGoalText() - dt);
	}

	if (GameManager::get_instance()->getDisplayGoalKickText() > 0) {
		GameManager::get_instance()->setDisplayGoalKickText(GameManager::get_instance()->getDisplayGoalKickText() - dt);
	}
}

void InfoPanel::render(World & world, Camera & camera, int screen_x, int screen_y)
{
	this->backgroundPanelRect.render(screen_x, screen_y);

	this->score.displayScore(screen_x, screen_y);

	if (GameManager::get_instance()->getDisplayGoalText() > 0) {
		this->score.displayGoal();
	}

	if (GameManager::get_instance()->getDisplayGoalKickText() > 0) {
		this->score.displayGoalKick();
	}

	this->miniMap.render(world, camera, screen_x + miniMap_x, screen_y + miniMap_y);

}
