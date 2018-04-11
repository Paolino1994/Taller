//
// Created by federico on 11/04/18.
//

#ifndef UNTITLED1_PLAYERVIEW_H
#define UNTITLED1_PLAYERVIEW_H


#include "Animation.h"
#include "PlayerModel.h"
#include <map>
#include <vector>

class PlayerView {

public:
    PlayerView(std::map<const std::string, Animation> &animationMapper, const player_data_t player_data,
                   PlayerModel* model);

    PlayerView(std::map<const std::string, Animation> map, PlayerModel model);

    PlayerView();

    void render(int screen_x, int screen_y);
private:
    std::vector<Animation> mAnimations;

    PlayerModel* playerModel;
};


#endif //UNTITLED1_PLAYERVIEW_H
