#ifndef SRC_CLIENT_GAMESELECTTEAM_H_
#define SRC_CLIENT_GAMESELECTTEAM_H_

#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <chrono>

#include "Animation.h"
#include "Texto.h"
#include "Texture.h"
#include "common/GameConstants.h"
#include "common/YAMLReader.h"
#include "Surface.h"
#include "common/Log.h"
#include "CommandSender.h"
#include "SoundManager.h"


class GameSelectTeam {

private:
    SDL_Renderer *gRenderer;
    int selectedTeam;

public:
    GameSelectTeam(SDL_Renderer *renderer);

    int selectTeamScreen(CommandSender& commandSender);

    int getSelectedTeam();


};

#endif /* SRC_CLIENT_GAMESELECTTEAM_H_ */
