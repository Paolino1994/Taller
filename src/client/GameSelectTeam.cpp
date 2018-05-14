#include "GameSelectTeam.h"

const int SCREEN_WIDTH = YAML::SCREEN_WIDTH;
const int SCREEN_HEIGHT = YAML::SCREEN_HEIGHT;

enum WriteState{
    TEAM = 0,
    ERROR
};

GameSelectTeam::GameSelectTeam(SDL_Renderer *renderer):
    	    gRenderer(renderer)
    	    {

    	    }

int GameSelectTeam::getSelectedTeam(){
	return selectedTeam;
}

int GameSelectTeam::selectTeamScreen() {
    Log *log = Log::get_instance();

    log->info("Generando pantalla de selección de equipo");

    Surface surface("res/soccer_field_1.6.png");

    Texture background(gRenderer, "res/soccer_field_1.6.png");

    background.render( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    int tituloW, tituloH, equipo, equipoH, errorH, errorW;

    WriteState state = WriteState::TEAM;

    // TODO - cambiar por algo mas copado: como mostrar la camiseta de los 2 equipos

    Texto tituloTxt(gRenderer, "res/Tehkan World Cup.ttf",36, "Tekhan Fiuba Cup", {255,255,0,0});
    Texto selectTeamTxt(gRenderer, "res/Tehkan World Cup.ttf",22, "Seleccione equipo: ", {255,255,0,0});

    tituloTxt.getTextureDimensions(&tituloW,&tituloH);
    selectTeamTxt.getTextureDimensions(&equipo,&equipoH);

    std::string teamText = " ";
    Texto teamTexture(gRenderer, "res/Tehkan World Cup.ttf",22,teamText , {255,255,0,0});

    std::string errorText = " ";
    Texto errorTxt(gRenderer, "res/Tehkan World Cup.ttf",22,errorText , {255,0,0,0});

    log->info("se crea las texturas para la selección de equipo");

    SDL_StartTextInput();

    log->info("inicio selección de equipo");

    int returnValue = 0;

    bool running = true;
    bool done = false;
    while ( running ) {
        SDL_Event ev;
        while ( SDL_PollEvent( &ev ) ) {
            if ( ev.type == SDL_TEXTINPUT ) {
                switch (state) {
                    case WriteState::TEAM:
                        if(teamText.compare(" ") == 0){
                            teamText = ev.text.text;
                        } else {
                            teamText += ev.text.text;
                        }
                    break;
                    case WriteState::ERROR:
                    break;
                    default:
                    break;
                }
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE && teamText.size()) {
                switch (state) {
                    case WriteState::TEAM:
                            teamText.pop_back();
                        break;
                        case WriteState::ERROR:
                        break;
                        default:
                        break;
                }
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN) {
                switch (state) {
                		case WriteState::TEAM:
                			done = true;
                        case WriteState::ERROR:
                            state = WriteState::TEAM;
//                            teamText = " ";
                            errorText = " ";
                        default:
                        break;
                }
            } else if ( ev.type == SDL_QUIT ) {
                running = false;
                returnValue = -1;
            }
        }

        SDL_RenderClear( gRenderer );
        background.render(0, 0);

        if(state == WriteState::TEAM) {
            teamTexture.updateText(teamText, {255,0,0,0});
        } else if (state == WriteState::ERROR){
            errorTxt.updateText(errorText, {255,0,0,0});
            errorTxt.getTextureDimensions(&errorW,&errorH);
            errorTxt.display((SCREEN_WIDTH - errorW) / 2, ((SCREEN_HEIGHT - errorH) / 4) * 3 );
        }
        tituloTxt.display((SCREEN_WIDTH - tituloW) / 2, (SCREEN_HEIGHT - tituloH) / 4);
        selectTeamTxt.display((SCREEN_WIDTH - equipo) / 3, (SCREEN_HEIGHT - equipoH) / 2);

        teamTexture.display(((SCREEN_WIDTH - equipo) / 3) + equipo, (SCREEN_HEIGHT - equipoH) / 2);

        if(done) {
            log->info("equipo seleccionado: " + teamText);
            selectedTeam = std::stoi( teamText );
            returnValue = selectedTeam;
            running = false;

        }
        SDL_RenderPresent( gRenderer );
    }

    SDL_StopTextInput();

    return returnValue;
}

