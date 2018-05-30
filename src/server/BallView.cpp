
#include "BallView.h"

BallView::BallView(std::map<const std::string, Animation>& animMapperBall, BallModel& ballModel):
	ballModel(ballModel),
	ballState(ballModel.getState())
{
    for(auto elem : animMapperBall)
        animation.push_back(elem.second);
}

void BallView::update(double deltaTime) {
	BallState newState = this->ballModel.getState();
	if (this->ballState != newState) {
		animation[newState].reset();
		this->ballState = newState;
	}
	
	animation[this->ballState].update(deltaTime);
}

void BallView::serialize(ball_view_data_t& ball_view_data) {
	ball_view_data.presentFrame = animation[ball_view_data.state].getPresentFrame();
}
