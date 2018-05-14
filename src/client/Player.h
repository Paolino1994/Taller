#pragma once

#include "Animation.h"
#include "Texture.h"
#include "Entity.h"
#include <map>
#include <vector>

class Player: public Entity {

public:
	Player(std::map<const std::string, Animation> animationMapper, player_data_t player_data, std::vector<Texture*> playerIndicators);

	void render(int screen_x, int screen_y);

	void update(const player_view_data_t& player_view_data);

	virtual int getWidth();
	virtual int getHeight();
	virtual double getAngle();

	PlayerState getState();

	bool isControlledByMe() const;
	void isControlledByMe(bool control);

private:
	std::vector<Animation> animations;
	double angle;
	PlayerState state;
	bool controlledByMe;
	std::vector<Texture*> indicators;
	int userId;
};
