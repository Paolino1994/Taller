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
    Log *log = Log::get_instance();

    log->info("Generando pantalla de selección de equipo");

    Texture background(gRenderer, "res/choose_team.jpg");

    background.render( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    int tituloW, tituloH;

    Texto tituloTxt(gRenderer, "res/Tehkan World Cup.ttf",36, "SELECCIONAR EQUIPO", {255,255,0,0});

    Texture localKit(gRenderer, "res/Rojo/shirt.png");
    Texture awayKit(gRenderer, "res/Verde/shirt.png");

    Texture selectTeam(gRenderer, "res/select_team.png");

    Animation selectTeamAnimation(selectTeam, 2, 2);

    tituloTxt.getTextureDimensions(&tituloW,&tituloH);

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
                team = Team::HOME;
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RIGHT) {
                team = Team::AWAY;
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN) {
                done = true;
            } else if ( ev.type == SDL_QUIT ) {
                running = false;
                returnValue = -1;
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

        localKit.render( 50, SCREEN_HEIGHT / 3);
        awayKit.render(SCREEN_WIDTH - 350 , SCREEN_HEIGHT / 3);

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

