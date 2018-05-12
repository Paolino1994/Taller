#include "SpriteInfoSetter.h"

SpriteInfoSetter::SpriteInfoSetter(int i) {
    if(i==BALL){
        setBallInfo();
    }else{
        equipo=i;
        setInfos();
    }

}

sprite_info SpriteInfoSetter::getPlayerRunInfo() {
	return PlayerRun;
}

sprite_info SpriteInfoSetter::getPlayerStillInfo() {
    return PlayerStill;
}

sprite_info SpriteInfoSetter::getPlayerSweepInfo() {
    return PlayerSweep;
}

sprite_info SpriteInfoSetter::getPlayerKickInfo() {
    return PlayerKick;
}

void SpriteInfoSetter::setInfos() {
    PlayerRun={"run",YAMLReader::get_instance().getSpriteRunning(equipo),60, 64,4,12};
    PlayerStill={"still",YAMLReader::get_instance().getSpriteStill(equipo),68, 34,1,3};
    PlayerSweep={"sweep",YAMLReader::get_instance().getSpriteSweeping(equipo),76, 112,4,12};
    PlayerKick={"kick",YAMLReader::get_instance().getSpriteKicking(equipo),74, 94,4,12};

}

sprite_info SpriteInfoSetter::getBallStillInfo() {
    return BallStill;
}

void SpriteInfoSetter::setBallInfo() {
    BallStill={"ballStill","res/Ball/ball_still.png",20, 20,1,3};
    BallMoving={"ballMoving","res/Ball/ball.png",20, 20,4,12};
}

sprite_info SpriteInfoSetter::getBallMovingInfo() {
    return BallMoving;
}

