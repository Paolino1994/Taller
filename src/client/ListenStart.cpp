#include "ListenStart.h"

const int SCREEN_WIDTH = YAML::WINDOW_WIDTH;
const int SCREEN_HEIGHT = YAML::WINDOW_HEIGHT;

enum WriteState{
    TEAM = 0,
    ERROR
};

ListenStart::ListenStart(SDL_Renderer *renderer):
            gRenderer(renderer)
            {

            }

void ListenStart::listenStartScreen(CommandSender& commandSender) {
    Log *log = Log::get_instance();

    log->info("Generando pantalla de espera a inicio del partidos");

    Surface surface("res/locker_room.jpg");

    Texture background(gRenderer, "res/locker_room.jpg");

    background.render( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    background.setScaling(YAML::WINDOW_WIDTH, YAML::WINDOW_HEIGHT);
    int tituloW, tituloH, start, startH;

    // TODO - cambiar por algo mas copado: como mostrar la camiseta de los 2 equipos

    Texto tituloTxt(gRenderer, "res/Tehkan World Cup.ttf",36, "Tekhan Fiuba Cup", {255,255,0,0});
    Texto listeningStartTxt(gRenderer, "res/Tehkan World Cup.ttf",22, "Esperando a los otros jugadores", {255,255,0,0});

    tituloTxt.getTextureDimensions(&tituloW,&tituloH);
    listeningStartTxt.getTextureDimensions(&start,&startH);

    log->info("se crea las texturas para espera del inicio del partido");

    SDL_RenderClear( gRenderer );
    background.render(0, 0);

    tituloTxt.display((SCREEN_WIDTH - tituloW) / 2, (SCREEN_HEIGHT - tituloH) / 4);
    listeningStartTxt.display((SCREEN_WIDTH - start) / 3, (SCREEN_HEIGHT - startH) / 2);

    SDL_RenderPresent( gRenderer );

	while (commandSender.listenStart() != Request::START);

	// Todos conectados, pido nombres de usuarios de la sesión actual

	commandSender.updateUsernames();
}

