#pragma once

#include "BallModel.h"
#include "Animation.h"


class BallView {
public:
    BallView(std::map<const std::string, Animation>& animMapperBall, BallModel& ballStill);

    void update(double d);

	void serialize(ball_view_data_t& ball_view_data);

private:
    std::vector<Animation> animation;

    BallModel& ballModel;

    BallState ballState;
};