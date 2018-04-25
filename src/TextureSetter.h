//
// Created by federico on 25/04/18.
//

#ifndef UNTITLED1_TEXTURESETTER_H
#define UNTITLED1_TEXTURESETTER_H


#include <SDL_render.h>
#include "GameConstants.h"
#include "Texture.h"

class TextureSetter {

public:
    TextureSetter(int i, SDL_Renderer *pRenderer);

    sprite_info getPlayerRunInfo();

    Texture getPLayerRunTexture();

    sprite_info getPlayerStillInfo();

    Texture getPLayerStillTexture();

    sprite_info getPlayerSweepInfo();

    Texture getPLayerSweepTexture();

    sprite_info getPlayerKickInfo();

    Texture getPLayerKickTexture();

private:

    int equipo;

    SDL_Renderer *gRenderer;

    sprite_info PlayerRun;

    sprite_info PlayerStill;

    sprite_info PlayerSweep;

    sprite_info PlayerKick;


    Texture setTextureRun();

    Texture setTextureStill();

    Texture setTextureSweep();

    Texture setTextureKick();

    void setInfos();
};


#endif //UNTITLED1_TEXTURESETTER_H
