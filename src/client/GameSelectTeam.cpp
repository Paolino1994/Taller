#include "GameSelectTeam.h"

const int SCREEN_WIDTH = YAML::SCREEN_WIDTH;
const int SCREEN_HEIGHT = YAML::SCREEN_HEIGHT;

typedef std::chrono::steady_clock Clock;

GameSelectTeam::GameSelectTeam(SDL_Renderer *renderer):
            gRenderer(renderer)
            {

            }

int GameSelectTeam::getSelectedTeam(){
    return selectedTeam;
}

int GameSelectTeam::selectTeamScreen(CommandSender& commandSender) {
    YAMLReader& yamlReader = YAMLReader::get_instance();

    Log *log = Log::get_instance();

    log->info("Generando pantalla de selección de equipo");

    Texture background(gRenderer, "res/choose_team.jpg");

    background.render( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    int tituloW, tituloH, localW, awayW, nameH;

    Texto tituloTxt(gRenderer, "res/Tehkan World Cup.ttf",36, "SELECCIONAR EQUIPO", {255,255,0,0});

    Texto nameLocalTxt(gRenderer, "res/Tehkan World Cup.ttf",36, yamlReader.getTeamNombre(1), {255,255,0,0});
    Texto nameAwayTxt(gRenderer, "res/Tehkan World Cup.ttf",36, yamlReader.getTeamNombre(2), {255,255,0,0});

    Texture localKit(gRenderer, "res/" + yamlReader.getTeamColor(1) + "/shirt.png");
    Texture awayKit(gRenderer, "res/" + yamlReader.getTeamColor(2) + "/shirt.png");

    Texture selectTeam(gRenderer, "res/select_team.png");

    Animation selectTeamAnimation(selectTeam, 2, 2);

    tituloTxt.getTextureDimensions(&tituloW,&tituloH);
    nameLocalTxt.getTextureDimensions(&localW,&nameH);
    nameAwayTxt.getTextureDimensions(&awayW,&nameH);


    log->info("se crea las texturas para la selección de equipo");

    int returnValue = 0;

    bool running = true;
    bool done = false;
    Team team = Team::HOME;

    Clock::time_point currentTime, newTime;
    currentTime = Clock::now();
    std::chrono::milliseconds milli;
    const double fixed_dt = 0.5; 
    double accumulator = 0;
    double frametime;

    while ( running ) {
        SDL_Event ev;
        while ( SDL_PollEvent( &ev ) ) {
            if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_LEFT) {
                if(team != Team::HOME) {
                    SoundManager::get_instance()->playSound(SoundEffect::SE_SELECT);
                    team = Team::HOME;
                }
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RIGHT) {
                if(team != Team::AWAY) {
                    SoundManager::get_instance()->playSound(SoundEffect::SE_SELECT);                
                    team = Team::AWAY;
                }
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN) {
                SoundManager::get_instance()->playSound(SoundEffect::SE_OK);                
                done = true;
            } else if ( ev.type == SDL_QUIT ) {
                running = false;
                returnValue = -1;
            } else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_p){
				SoundManager::get_instance()->musicOn_off();
			} else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_o) {
                SoundManager::get_instance()->soundEffectsOn_off();
            }
        }

        SDL_RenderClear( gRenderer );
        background.render(0, 0);

        newTime = Clock::now();
        milli = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime);
        currentTime = newTime;
        frametime = milli.count()/1000.0;

        accumulator += frametime;

        if (accumulator >= fixed_dt)
        {
            selectTeamAnimation.update(fixed_dt);
            accumulator = 0;
        }

       
        tituloTxt.display((SCREEN_WIDTH - tituloW) / 2, (SCREEN_HEIGHT - tituloH) / 8);

        nameLocalTxt.display((SCREEN_WIDTH / 4) - (localW / 2), SCREEN_HEIGHT / 3);
        nameAwayTxt.display(SCREEN_WIDTH - (SCREEN_WIDTH / 4) - (awayW / 2), SCREEN_HEIGHT / 3);

        localKit.render( (SCREEN_WIDTH / 2) - 350, (SCREEN_HEIGHT / 2) - 25);
        awayKit.render((SCREEN_WIDTH / 2) + 50 , (SCREEN_HEIGHT / 2)  - 25);

        if(team == Team::HOME){
            selectTeamAnimation.render(20, (SCREEN_HEIGHT / 3) - 20);
        } else if (team == Team::AWAY) {
            selectTeamAnimation.render(SCREEN_WIDTH - 380, (SCREEN_HEIGHT / 3) - 20);
        }

        if(done) {
            log->info("se selecciono un equipo.");
            returnValue = 0;
            running = false;
            commandSender.assignTeam(team);
        }
        SDL_RenderPresent( gRenderer );
    }

    return returnValue;
}

