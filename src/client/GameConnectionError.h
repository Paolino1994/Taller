#ifndef SRC_CLIENT_GAMECONNECTIONERROR_H_
#define SRC_CLIENT_GAMECONNECTIONERROR_H_


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

class GameConnectionError {

private:
	SDL_Renderer *gRenderer;
public:
	GameConnectionError(SDL_Renderer *renderer);

	void connectionErrorScreen();
};

#endif /* SRC_CLIENT_GAMECONNECTIONERROR_H_ */
