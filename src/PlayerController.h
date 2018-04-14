//
// Created by federico on 11/04/18.
//

#ifndef UNTITLED1_PLAYERCONTROLLER_H
#define UNTITLED1_PLAYERCONTROLLER_H


#include "PlayerModel.h"
#include "PlayerView.h"

class PlayerController {

public:
    PlayerController(PlayerModel* model, PlayerView* view):
    	playerModel(model),
		playerView(view){};

    Entity* getEntity() {
    	return playerModel;
    }

    PlayerView* getView() {
       	return playerView;
    }

    void handleEvent(SDL_Event &e);

	void update(double dt, int x_limit, int y_limit);

	void swap(PlayerController* otherController);

private:

	PlayerModel * playerModel;
	PlayerView *playerView;

};


#endif //UNTITLED1_PLAYERCONTROLLER_H
