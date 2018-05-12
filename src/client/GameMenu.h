#pragma once
#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Texto.h"
#include "Texture.h"
#include "Surface.h"
#include "CommandSender.h"
#include "common/Log.h"

class GameMenu {
private:
    SDL_Renderer *gRenderer;
public:
    GameMenu(SDL_Renderer *renderer);

    int verificarCredenciales (std::string usuario, std::string pass);

    int logginScreen(CommandSender& commandSender);
};
