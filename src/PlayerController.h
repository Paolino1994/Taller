//
// Created by federico on 11/04/18.
//

#ifndef UNTITLED1_PLAYERCONTROLLER_H
#define UNTITLED1_PLAYERCONTROLLER_H


#include "PlayerModel.h"
#include "PlayerView.h"

class PlayerController {

public:
    PlayerController(PlayerModel* model, PlayerView* view);

    void handleEvent(SDL_Event &e);

private:

    PlayerModel *playerModel;
    PlayerView *playerView;
};


#endif //UNTITLED1_PLAYERCONTROLLER_H
