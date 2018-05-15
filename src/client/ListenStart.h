#ifndef SRC_CLIENT_LISTENSTART_H_
#define SRC_CLIENT_LISTENSTART_H_

#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Texto.h"
#include "Texture.h"
#include "common/GameConstants.h"
#include "Surface.h"
#include "CommandSender.h"

#include "common/Log.h"
#include "common/Request.h"

class ListenStart {

private:
    SDL_Renderer *gRenderer;
    int selectedTeam;

public:
    ListenStart(SDL_Renderer *renderer);

    void listenStartScreen(CommandSender& commandSender);

};

#endif /* SRC_CLIENT_LISTENSTART_H_ */
