#pragma once

#include "common/GameConstants.h"
#define BALL 3


// Re-Pensar bien esta clase!!!!
class SpriteInfoSetter {

public:
	SpriteInfoSetter(int i); // en vez de int, Enum de algo!

    sprite_info getPlayerRunInfo();

    sprite_info getPlayerStillInfo();

    sprite_info getPlayerSweepInfo();

    sprite_info getPlayerKickInfo();

    sprite_info getBallStillInfo();

    sprite_info getBallMovingInfo();

private:

    int equipo;  // en vez de int, Enum de algo!

    sprite_info PlayerRun;

    sprite_info PlayerStill;

    sprite_info PlayerSweep;

    sprite_info PlayerKick;

    sprite_info BallStill;

    sprite_info BallMoving;

    void setInfos();

    void setBallInfo();

};