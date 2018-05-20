#include "GameConnectionError.h"

const int SCREEN_WIDTH = YAML::SCREEN_WIDTH;
const int SCREEN_HEIGHT = YAML::SCREEN_HEIGHT;

GameConnectionError::GameConnectionError(SDL_Renderer *renderer):
gRenderer(renderer)
{

}

void GameConnectionError::connectionErrorScreen(){
	Log *log = Log::get_instance();

	    log->info("Generando pantalla de caida de servidor");

	    Texture background(gRenderer, "res/choose_team.jpg");

	    background.render( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	    int tituloW, tituloH, start, startH;

	    Texto tituloTxt(gRenderer, "res/Tehkan World Cup.ttf",36, "Tekhan Fiuba Cup", {255,255,0,0});
	    Texto listeningStartTxt(gRenderer, "res/Tehkan World Cup.ttf",22, "Error: servidor no disponible", {255,255,0,0});

	    tituloTxt.getTextureDimensions(&tituloW,&tituloH);
	    listeningStartTxt.getTextureDimensions(&start,&startH);

	    SDL_RenderClear( gRenderer );
	    background.render(0, 0);

	    tituloTxt.display((SCREEN_WIDTH - tituloW) / 2, (SCREEN_HEIGHT - tituloH) / 4);
	    listeningStartTxt.display((SCREEN_WIDTH - start) / 3, (SCREEN_HEIGHT - startH) / 2);

	    SDL_RenderPresent( gRenderer );

	    bool running = true;
	    while ( running ) {
            SDL_Event event;
            while ( SDL_PollEvent( &event ) ) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
					running = false;
					break;
				}
            }
	    }
}

