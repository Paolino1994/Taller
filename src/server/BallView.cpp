//
// Created by federico on 30/04/18.
//

#include "BallView.h"
#include "BallController.h"

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

/*
void BallView::render(int i, int i1,double angle) {
    int xCorrection=ballModel.getXCorrection();
    int yCorrection=ballModel.getYCorrection();

    animation[priorState].render(i+xCorrection,i1+yCorrection,angle);
}

void BallView::render(int screen_x, int screen_y) {
    int angle=ballModel.getAngle();
    animation[priorState].render(screen_x,screen_y,angle);
}*/


void BallView::serialize(ball_view_data_t& ball_view_data) {
	ball_view_data.presentFrame = animation[ball_view_data.state].getPresentFrame();
}




