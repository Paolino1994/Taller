#include "Ball.h"

Ball::Ball(std::map<const std::string, Animation>& animMapperBall, Texture * miniMapIndicator):
	Entity(),
	angle(0.0),
	state(QUIESCENT),
	miniMapIndicator(miniMapIndicator)
{
	for (auto elem : animMapperBall) {
		animations.push_back(elem.second);
	}
}

void Ball::render(int screen_x, int screen_y) {
	animations[state].render(screen_x, screen_y, angle,z);
	miniMapIndicator->render(this->x / 10, this->y /10);
}

void Ball::update(const ball_view_data_t & ball_view_data)
{
	this->x = ball_view_data.x;
	this->y = ball_view_data.y;
    this->z = ball_view_data.z;
	this->angle = ball_view_data.angle;
	this->state = ball_view_data.state;
	this->animations[this->state].setPresentFrame(ball_view_data.presentFrame);
}

int Ball::getWidth()
{
	return animations[state].getDestinationWidth();
}

int Ball::getHeight()
{
	return animations[state].getDestinationHeight();
}

double Ball::getAngle()
{
	return this->angle;
}

BallState Ball::getState()
{
	return this->state;
}





