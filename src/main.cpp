#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <map>

#include "World.h"
#include "Texture.h"
#include "Surface.h"
#include "Animation.h"
#include "Entity.h"
#include "Camera.h"
#include "GameConstants.h"
#include "Player.h"
#include "Log.h"
#include "PlayerModel.h"
#include "PlayerView.h"
#include "PlayerController.h"
#include "TeamFactory.h"
#include "YAMLReader.h"

//Screen dimension constants
const int SCREEN_WIDTH = YAML::SCREEN_WIDTH;
const int SCREEN_HEIGHT = YAML::SCREEN_HEIGHT;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

void setPlayerRun(YAMLReader reader);

//Starts up SDL and creates window
bool init_SDL()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

//Frees media and shuts down SDL
void close()
{
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

typedef std::chrono::steady_clock Clock;

int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if ( !init_SDL() ) {
        std::cout << "Failed to initialize!\n" << std::endl;
    } else {
        std::map<const std::string, Animation> animMapper;

        // Inicializar log con parametro de line de comando
        for (int i = 1; i+1 < argc; i++) {
            if (strcmp(args[i],"-lg") == 0) {
                Log::initialize(args[i + 1]);
            }
        }
        if (!Log::is_initialized()) {
            Log::initialize(LOG_DEBUG);
        }

        // CARGAR La configuracion del YAML y de constantes nuestras:
		// TODO

        //Background:
        Texture background(gRenderer, YAML::background_path);

        //Las texturas:
		Surface runS(YAML::PlayerRun.file_path);
		runS.setColorKey(126, 130, 56); //cargar desde constantes
        Texture runT(gRenderer, runS);
        runT.setScaling(YAML::PlayerRun.width, YAML::PlayerRun.height);

		Surface stillS(YAML::PlayerStill.file_path);
		stillS.setColorKey(126, 130, 56); //cargar desde constantes
        Texture stillT(gRenderer, stillS);
        stillT.setScaling(YAML::PlayerStill.width, YAML::PlayerStill.height);

        Surface sweepS(YAML::PlayerSweep.file_path);
        sweepS.setColorKey(126, 130, 56); //cargar desde constantes
		Texture sweepT(gRenderer, sweepS);
		sweepT.setScaling(YAML::PlayerSweep.width, YAML::PlayerSweep.height);

        Surface kickS(YAML::PlayerKick.file_path);
        kickS.setColorKey(126, 130, 56); //cargar desde constantes
        Texture kickT(gRenderer, kickS);
        kickT.setScaling(YAML::PlayerKick.width, YAML::PlayerKick.height);

        //YAMLReader reader("GeneralConfig.yaml");
        //YAML::PlayerRun.file_path=reader.getSpriteRunning(EQUIPO1);
        //YAML::PlayerStill.file_path=reader.getSpriteStill(EQUIPO1);

		// Crear animaciones en base a datos del sprite y mandarlos a un map para el Player
        animMapper.emplace(std::make_pair(YAML::PlayerRun.spriteid, Animation(runT, YAML::PlayerRun)));
        animMapper.emplace(std::make_pair(YAML::PlayerStill.spriteid, Animation(stillT, YAML::PlayerStill)));
        animMapper.emplace(std::make_pair(YAML::PlayerSweep.spriteid, Animation(sweepT, YAML::PlayerSweep)));
        animMapper.emplace(std::make_pair(YAML::PlayerKick.spriteid, Animation(kickT, YAML::PlayerKick)));

        // Creo jugadores:
		TeamFactory* tfactory = new TeamFactory();
		tfactory->create(3, 2, 1, LEFT_GOAL, background.getWidth(), background.getHeight());
		tfactory->add_view(animMapper);

		// Agrego jugadores al mundo
        World world(background.getWidth(), background.getHeight(), &background);
		tfactory->add_to_world(world);

		// Asigno jugador a ser controlado
		std::vector<player>::iterator teamIterator = std::prev(tfactory->get_team().end());
		PlayerController* controlled = teamIterator->controller;

        Camera camera(world, SCREEN_WIDTH, SCREEN_HEIGHT, YAML::SCREEN_WIDTH_SCROLL_OFFSET, YAML::SCREEN_HEIGHT_SCROLL_OFFSET);
        camera.follow(teamIterator->model);

        if (true)
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //El tema del clock, es para actualizar en intervalos fijos
            //Y renderizar en el resto del tiempo
            Clock::time_point currentTime, newTime;
            currentTime = Clock::now();
            std::chrono::milliseconds milli;
            const double fixed_dt = 0.01; //10 milliseconds
            double accumulator = 0;
            double frametime;

            //While application is running
            while( !quit )
            {
                newTime = Clock::now();
                milli = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime);
                currentTime = newTime;
                frametime = milli.count()/1000.0;

                accumulator += frametime;

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0)
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }

					if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) {
						// sabemos que el iterador apunta al controlled actual
						player& controlledPlayer = *teamIterator;
						do
						{ // en el peor caso volvemos a el que estabamos controlando recien
							++teamIterator;
							if (teamIterator == tfactory->get_team().end()) {
								teamIterator = tfactory->get_team().begin();
							}

						} while (teamIterator->controller != controlled && !camera.isWithinScrollBoundaries(teamIterator->model));
						
						if (teamIterator->controller != controlled) {
							controlled->swap(teamIterator->controller);
							camera.follow(controlled->getEntity());
							// esto es un parche medio feo por los lios del swap, TODO mejorar
							controlledPlayer.controller = teamIterator->controller;
							teamIterator->controller = controlled;
						}
					}

                    controlled->handleEvent(e);
                }

                //Cuando el tiempo pasado es mayor a nuestro tiempo de actualizacion
                while ( accumulator >= fixed_dt )
                {
                    //Calculate movement/physics:
					world.update(fixed_dt); //Update de todos los players (y otras entidades proximamente?)
                    camera.update(fixed_dt);
                    accumulator -= fixed_dt;
                }

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                //Render current frame
                camera.render(world);	

                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}


