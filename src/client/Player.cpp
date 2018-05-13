#include "Player.h"

Player::Player(std::map<const std::string, Animation> animationMapper, player_data_t player_data):
	Entity(),
	animations(std::vector<Animation>()),
	angle(0.0),
	state(PlayerState::STILL),
	controlledByMe(false)
{
	this->animations.reserve(PlayerState::_LENGTH_);
    for(int i=0; i<PlayerState::_LENGTH_; i++){
        // copias de animations
        this->animations.push_back(animationMapper.at(player_data.sprite_ids[i]));
    }
}

void Player::render(int screen_x, int screen_y)
{
    animations[state].render(screen_x, screen_y, angle);
}

void Player::update(const player_view_data_t & player_view_data)
{
	// Por ahora sin tocar playerId o Team

	this->x = player_view_data.x;
	this->y = player_view_data.y;
	this->angle = player_view_data.angle;
	this->state = player_view_data.state;
	this->animations[this->state].setPresentFrame(player_view_data.presentFrame);
}

int Player::getWidth()
{
	return animations[state].getDestinationWidth();
}

int Player::getHeight()
{
	return animations[state].getDestinationHeight();
}

double Player::getAngle()
{
	return this->angle;
}

PlayerState Player::getState()
{
	return this->state;
}

bool Player::isControlledByMe() const 
{
	return this->controlledByMe;
}

void Player::isControlledByMe(bool control)
{
	this->controlledByMe = control;
}






