//
// Created by federico on 11/04/18.
//


#include "PlayerView.h"
#include "PlayerModel.h"


PlayerView::PlayerView(std::map<const std::string, Animation>& animationMapper, const player_data_t player_data, PlayerModel *model){
    this->mAnimations.reserve(_LENGTH_);
    playerModel=model;
    for(int i=0; i<PlayerState::_LENGTH_; i++){
        // copias de animations
        this->mAnimations.push_back(animationMapper.at(player_data.sprite_ids[i]));
    }
}

void PlayerView::render(int screen_x, int screen_y)
{
    mAnimations[playerModel->getState()].render(screen_x, screen_y, playerModel->getAngle());
}






