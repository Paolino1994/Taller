#pragma once
#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Texto.h"
#include "Texture.h"
#include "Surface.h"
#include "CommandSender.h"
#include "common/Log.h"
using namespace std;

class GameMenu {
private:
    SDL_Renderer *gRenderer;
public:
    GameMenu(SDL_Renderer *renderer);

    int verificarCredenciales (std::string usuario, std::string pass);

    int logginScreen(CommandSender& commandSender);

    bool pausaMenu(SDL_Event ev);

};
