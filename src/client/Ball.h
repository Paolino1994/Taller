#pragma once

#include "Entity.h"
#include "Texture.h"
#include "Animation.h"


class Ball : public Entity {
public:
	Ball(std::map<const std::string, Animation>& animMapperBall, Texture *miniMapIndicator);

	void render(int screen_x,int screen_y);

    void update(const ball_view_data_t& ball_view_data);

	virtual int getWidth();
	virtual int getHeight();
	virtual double getAngle();

	BallState getState();

private:
    std::vector<Animation> animations;
	double angle;
	BallState state;
	Texture* miniMapIndicator;
};