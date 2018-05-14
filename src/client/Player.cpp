#include "Player.h"

Player::Player(std::map<const std::string, Animation> animationMapper, player_data_t player_data, std::vector<Texture*> playerIndicators):
	Entity(),
	animations(std::vector<Animation>()),
	angle(0.0),
	state(PlayerState::STILL),
	controlledByMe(false),
	indicators(std::vector<Texture*>())
{
	this->animations.reserve(PlayerState::_LENGTH_);
	this->indicators.reserve(4); // REFACTOR
    for(int i=0; i<PlayerState::_LENGTH_; i++){
        // copias de animations
        this->animations.push_back(animationMapper.at(player_data.sprite_ids[i]));
    }
	this->indicators.push_back(playerIndicators[0]);
	for(int i=0; i<4; i++){
		this->indicators.push_back(playerIndicators[i]);
    }
	this->userId = -1;
}

void Player::render(int screen_x, int screen_y)
{
	if(this->userId != -1){
		indicators[this->userId]->render(screen_x, screen_y);
	}
    animations[state].render(screen_x, screen_y, angle);
}

void Player::update(const player_view_data_t & player_view_data)
{
	// Por ahora sin tocar playerId o Team

	this->x = player_view_data.x;
	this->y = player_view_data.y;
	this->angle = player_view_data.angle;
	this->state = player_view_data.state;
	this->userId = player_view_data.userId;
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






