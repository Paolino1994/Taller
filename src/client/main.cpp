#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <map>

#include "World.h"
#include "Animation.h"
#include "common/GameConstants.h"

#include "Texture.h"
#include "Surface.h"

#include "Camera.h"

#include "common/SocketException.h"
#include "common/Log.h"
#include "common/YAMLReader.h"
#include "Texto.h"
#include "TextureSetter.h"
#include "CommandSender.h"
#include "GameMenu.h"
#include "GameSelectTeam.h"
#include "ListenStart.h"
#include "GameConnectionError.h"

#include "SoundManager.h"
#include "GameManager.h"
#include "../common/GameConstants.h"


//Screen dimension constants
const int SCREEN_WIDTH = YAML::SCREEN_WIDTH;
const int SCREEN_HEIGHT = YAML::SCREEN_HEIGHT;

enum GameState {
    OFFLINE = 0,
    ONLINE
};

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

std::string getLogType(char *string);

void renderizar(Camera& camera, World& world, CommandSender& commandSender, GameMenu& gameMenu);

player_data_t
crearDefaultPlayer(sprite_info PlayerStill, sprite_info PlayerRun, sprite_info PlayerSweep, sprite_info PlayerKick);

//Starts up SDL and creates window
bool init_SDL()
{
    //Initialization flag
    bool success = true;
	Log* log = Log::get_instance();
	std::stringstream msg;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        // printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		msg << "SDL could not initialize! SDL Error: " << SDL_GetError();
		log->error(msg.str());
		msg.str(std::string());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" ) )
        {
            // printf( "Warning: Linear texture filtering not enabled!" );
			log->error("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow( "TEHKAN FIUBA CUP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        							SCREEN_WIDTH, SCREEN_HEIGHT + YAML::MINIMAP_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            // printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			msg << "Window could not be created! SDL Error: " << SDL_GetError();
			log->error(msg.str());
			msg.str(std::string());
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                // printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				msg << "Renderer could not be created! SDL Error: " << SDL_GetError();
				log->error(msg.str());
				msg.str(std::string());
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
                    // printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					msg << "SDL_image could not initialize! SDL Error: " << IMG_GetError();
					log->error(msg.str());
					msg.str(std::string());
                    success = false;
                }
            }
        }
    }
    if(TTF_Init() < 0){
        // printf( "TTF could not initialize! SDL Error: %s\n", SDL_GetError() );
		msg << "TTF could not initialize! SDL Error: " << SDL_GetError();
		log->error(msg.str());
		msg.str(std::string());
        success = false;
    }
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
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
    TTF_Quit();
    Mix_Quit();
    Mix_CloseAudio();


}

typedef std::chrono::steady_clock Clock;

int main( int argc, char* args[] )
{
    std::string yamlConfigFile = "";
	std::string server_ip = "127.0.0.1";
	std::string server_port = "5000";
    // Inicializar log con parametro de line de comando
    for (int i = 1; i+1 < argc; i++) {
        if (strcmp(args[i],"-lg") == 0) {
            std::string logType=getLogType(args[i+1]);
            if(logType.compare("NOT_FOUND") != 0) {
                Log::initialize(logType);
                Log* log=Log::get_instance();
                log->error("Log cargado en modo " + logType);
            } else {
                Log::initialize(LOG_ERROR);
                Log* log=Log::get_instance();
                log->error("Log ingresado inválido. Log cargado en modo error");
            }
        } else if (strcmp(args[i],"-yaml") == 0) {
            yamlConfigFile = args[i+1];
        } else if (strcmp(args[i], "-ip") == 0) {
			server_ip = args[i + 1];
		} else if (strcmp(args[i], "-port") == 0) {
			server_port = args[i + 1];
		}
    }
    YAMLReader& yamlReader = YAMLReader::get_instance();
    yamlReader.readYamlGeneral(yamlConfigFile);
    if (!Log::is_initialized()) {
        std::string logLevel = yamlReader.getLogLevel();
        std::string logType=getLogType((char *) logLevel.c_str());
		if(logType.compare("NOT_FOUND") != 0) {
            Log::initialize(logType);
            Log* log=Log::get_instance();
            log->error("Log cargado en modo " + logType);
        } else {
            Log::initialize(LOG_ERROR);
            Log* log=Log::get_instance();
            log->error("Log en archivo de configuración inválido. Log cargado en modo error");
        }
    }
    
	Log* log = Log::get_instance();
    
    GameState gameState = GameState::OFFLINE;

    //Start up SDL and create window
    if ( !init_SDL() ) {
        std::cout << "Failed to initialize!\n" << std::endl;
		log->error("Falló la inicialización del SDL");
    } else {
	    std::unique_ptr<CommandSender> commandSenderPtr(nullptr);

        SoundManager::get_instance()->startGameMusic();

		try {
			commandSenderPtr = std::unique_ptr<CommandSender>(new CommandSender(server_ip, std::stoul(server_port, nullptr, 0)));
		}
		catch (SocketException& ex) {
			//std::cout << "No nos pudimos conectar al servidor. Ver el log" << std::endl;
			Log::get_instance()->error("No nos pudimos conectar al servidor, causa: " + std::string(ex.what()));
			GameConnectionError gameConnectionError(gRenderer);
			gameConnectionError.connectionErrorScreen();
			close();
			return 1;
		}

        GameMenu gameMenu(gRenderer);
        Team selectedTeam;
        if(gameState == GameState::OFFLINE) {
			try {
				CommandSender& commandSender = *commandSenderPtr;
				if (gameMenu.logginScreen(commandSender) == 0) {
					gameState = GameState::ONLINE;
					// selecionar equipo
					GameSelectTeam gameSelectTeam(gRenderer);
					if (gameSelectTeam.selectTeamScreen(commandSender) == 0) {
						selectedTeam = gameSelectTeam.getSelectedTeam();
						log->info("se seleccionó correctamente el equipo");
					}

				}
			}
			catch (SocketException& ex){
				//std::cout << "Error de conexión con el servidor. Salimos. Ver el log" << std::endl;
				Log::get_instance()->error("Error de conexión con el servidor, causa: " + std::string(ex.what()));
				GameConnectionError gameConnectionError(gRenderer);
				gameConnectionError.connectionErrorScreen();
				close();
				return 1;
			}
        }
        if (gameState == GameState::ONLINE) {

            CommandSender& commandSender = *commandSenderPtr;
            gameMenu.selectFormationScreen(commandSender, selectedTeam);

            ListenStart listenStart(gRenderer);
            listenStart.listenStartScreen(commandSender);

			/****************************************
			** INICIO CREACION TEXTURAS Y ANIMACIONES
			*/
            std::map<const std::string, Animation> animMapperHOME;
            std::map<const std::string, Animation> animMapperAWAY;
            std::map<const std::string, Animation> animMapperBall;
            std::vector<Texture*> playerIndicators;
            // CARGAR La configuracion del YAML y de constantes nuestras:
            // TODO

            //Background:
            Texture background(gRenderer, YAML::background_path);
            // Jugador Seleccionado:
            Texture indicatorRed(gRenderer, YAML::indicators_path + "/red.png");
            Texture indicatorBlue(gRenderer, YAML::indicators_path + "/blue.png");
            Texture indicatorYellow(gRenderer, YAML::indicators_path + "/yellow.png");
            Texture indicatorPink(gRenderer, YAML::indicators_path + "/pink.png");

            playerIndicators.push_back(&indicatorRed);
            playerIndicators.push_back(&indicatorBlue);
            playerIndicators.push_back(&indicatorYellow);
            playerIndicators.push_back(&indicatorPink);

            //Las texturas:
            log->info("Cargando Texturas");
            
            // EQUIPO 1 HAY QUE MODULARIZAR ESTO 
            int equipo=1;
            log->info("Crear Texturas");
            TextureSetter textures(equipo, gRenderer);
            sprite_info PlayerRun=textures.getPlayerRunInfo();
            Texture runT=textures.getPLayerRunTexture();
            sprite_info PlayerStill=textures.getPlayerStillInfo();
            Texture stillT=textures.getPLayerStillTexture();
            sprite_info PlayerSweep=textures.getPlayerSweepInfo();
            Texture sweepT=textures.getPLayerSweepTexture();
            sprite_info PlayerKick=textures.getPlayerKickInfo();
            Texture kickT=textures.getPLayerKickTexture();
            // Crear animaciones en base a datos del sprite y mandarlos a un map para el Player
            log->info("Crear Animaciones");
            log->debug("Crear Animacion Run");
            animMapperHOME.emplace(std::make_pair(PlayerRun.spriteid, Animation(runT, PlayerRun)));
            log->debug("Crear Animacion Still");
            animMapperHOME.emplace(std::make_pair(PlayerStill.spriteid, Animation(stillT, PlayerStill)));
            log->debug("Crear Animacion Sweep");
            animMapperHOME.emplace(std::make_pair(PlayerSweep.spriteid, Animation(sweepT, PlayerSweep)));
            log->debug("Crear Animacion Kick");
            animMapperHOME.emplace(std::make_pair(PlayerKick.spriteid, Animation(kickT, PlayerKick)));

            // Textura de minimapa
            Texture miniMapIndicatorHome(gRenderer, "res/" + yamlReader.getTeamColor(1) + "/mini_map_indicatorBlue.png");

            TextureSetter textures2(2, gRenderer);
            sprite_info PlayerRun2=textures2.getPlayerRunInfo();
            Texture runT2=textures2.getPLayerRunTexture();
            sprite_info PlayerStill2=textures2.getPlayerStillInfo();
            Texture stillT2=textures2.getPLayerStillTexture();
            sprite_info PlayerSweep2=textures2.getPlayerSweepInfo();
            Texture sweepT2=textures2.getPLayerSweepTexture();
            sprite_info PlayerKick2=textures2.getPlayerKickInfo();
            Texture kickT2=textures2.getPLayerKickTexture();
            // Crear animaciones en base a datos del sprite y mandarlos a un map para el Player
            log->info("Crear Animaciones del equipo 2");
            log->debug("Crear Animacion Run");
            animMapperAWAY.emplace(std::make_pair(PlayerRun2.spriteid, Animation(runT2, PlayerRun2)));
            log->debug("Crear Animacion Still");
            animMapperAWAY.emplace(std::make_pair(PlayerStill2.spriteid, Animation(stillT2, PlayerStill2)));
            log->debug("Crear Animacion Sweep");
            animMapperAWAY.emplace(std::make_pair(PlayerSweep2.spriteid, Animation(sweepT2, PlayerSweep2)));
            log->debug("Crear Animacion Kick");
            animMapperAWAY.emplace(std::make_pair(PlayerKick2.spriteid, Animation(kickT2, PlayerKick2)));
            // Textura de minimapa
            Texture miniMapIndicatorAway(gRenderer, "res/" + yamlReader.getTeamColor(2) + "/mini_map_indicator.png");

			log->info("Crear Animaciones de la pelota");
			TextureSetter texturesBall(BALL, gRenderer);
			sprite_info ballStill = texturesBall.getBallStillInfo();
			Texture ballStillT = texturesBall.getBallStillTexture();
			sprite_info ballMoving = texturesBall.getBallMovingInfo();
			Texture ballMovingT = texturesBall.getBallMovingTexture();
			//sprite_info ballStill=texturesBall.getBallStillInfo();
			animMapperBall.emplace(std::make_pair(ballStill.spriteid, Animation(ballStillT, ballStill)));
			animMapperBall.emplace(std::make_pair(ballMoving.spriteid, Animation(ballMovingT, ballMoving)));

            Texture miniMapIndicatorBall(gRenderer, "res/Ball/mini_map_indicator.png");

            Texture miniCamera(gRenderer, "res/camera_rect.png");
            miniCamera.setScaling(70, 70);
            Texture miniField(gRenderer, "res/soccer_field_1.6.png");
            miniField.setScaling(YAML::MINIMAP_WIDTH, YAML::MINIMAP_HEIGHT - 50);

            Texture backgroundPanel(gRenderer, "res/backgroundPanel.jpg");
            backgroundPanel.setScaling(SCREEN_WIDTH, YAML::MINIMAP_HEIGHT - 50);

            Texto scoreHomeName(gRenderer, "res/Tehkan World Cup.ttf",22, yamlReader.getTeamNombre(1) , {255,255,0,0});
            Texto scoreAwayName(gRenderer, "res/Tehkan World Cup.ttf",22, yamlReader.getTeamNombre(2) , {255,255,0,0});
            Texto scoreHome(gRenderer, "res/Tehkan World Cup.ttf",50, "0", {255,255,0,0});
            Texto scoreAway(gRenderer, "res/Tehkan World Cup.ttf",50, "0", {255,255,0,0});
            Texto tiempo(gRenderer, "res/Tehkan World Cup.ttf",50, "0", {255,255,0,0});


            Score score(gRenderer, &scoreHomeName, &scoreAwayName, &scoreHome, &scoreAway,&tiempo);
            score.initialize();


            /*
            ** FIN CREACION TEXTURAS Y ANIMACIONES
            **************************************/


			// Agrego jugadores al mundo
			log->info("Creo el mundo con su pelota");
			World world(background.getWidth(), background.getHeight(), &background, playerIndicators, animMapperBall, &miniMapIndicatorBall, animMapperHOME, animMapperAWAY, &miniMapIndicatorHome, &miniMapIndicatorAway,
						crearDefaultPlayer(PlayerStill, PlayerRun, PlayerSweep, PlayerKick));

            
			log->info("Agrego la camara");
            Camera camera(world, SCREEN_WIDTH, SCREEN_HEIGHT, YAML::SCREEN_WIDTH_SCROLL_OFFSET, YAML::SCREEN_HEIGHT_SCROLL_OFFSET, &miniCamera, &miniField, &backgroundPanel, &score);
            camera.follow(world.getBall());

            log->info("Renderizo");
			try {
				CommandSender& commandSender = *commandSenderPtr;
				renderizar(camera, world, commandSender, gameMenu);
			} catch (SocketException& ex) {
                // pantalla que muestra la desconexion
				//std::cout << "Error de conexión con el servidor. Salimos. Ver el log" << std::endl;
				Log::get_instance()->error("Error de conexión con el servidor, causa: " + std::string(ex.what()));
				GameConnectionError gameConnectionError(gRenderer);
				gameConnectionError.connectionErrorScreen();
                close();
				return 1;
			}
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}

player_data_t crearDefaultPlayer(sprite_info PlayerStill, sprite_info PlayerRun, sprite_info PlayerSweep, sprite_info PlayerKick) {
    player_data_t defaultPlayer = {{
                                           [PlayerState::STILL] = PlayerStill.spriteid,
                                           [PlayerState::RUNNING] = PlayerRun.spriteid,
                                           [PlayerState::SWEEPING] = PlayerSweep.spriteid,
                                           [PlayerState::KICKING] = PlayerKick.spriteid,

                                   },

            /*WIDTHS*/  {
                                           [PlayerState::STILL] = PlayerStill.width,
                                           [PlayerState::RUNNING] = PlayerRun.width,
                                           [PlayerState::SWEEPING] = PlayerSweep.width,
                                           [PlayerState::KICKING] = PlayerKick.width,
                                   },
            /*HEIGHTS*/  {
                                           [PlayerState::STILL] = PlayerStill.height,
                                           [PlayerState::RUNNING] = PlayerRun.height,
                                           [PlayerState::SWEEPING] = PlayerSweep.height,
                                           [PlayerState::KICKING] = PlayerKick.height,
                                   },

            // pixeles (logicos) por segundo
            /*X_VELOCITY =*/ 200,
            /*Y_VELOCITY =*/ 200,
            /*SWEEP_DURATION =*/ (1.0 / PlayerSweep.frames_per_second) * PlayerSweep.frames,
            /*KICK_DURATION =*/ (1.0 / PlayerKick.frames_per_second) * PlayerKick.frames,
            /*SPRINT_VEL_MULT*/ 2.0
    };
    return defaultPlayer;
}

void renderizar(Camera& camera, World& world, CommandSender& commandSender, GameMenu& gameMenu) {

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
        const double fixed_dt = 0.005; //10 milliseconds
        double accumulator = 0;
        double frametime;
        bool salirJuego = false;

		Log* log = Log::get_instance();
	
	commandSender.set_rcv_timeout(30);

        //While application is running
        while( !quit )
        {
            newTime = Clock::now();
            milli = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime);
            currentTime = newTime;
            frametime = milli.count()/1000.0;

            accumulator += frametime;

            //Handle events on queue
            while (SDL_PollEvent( &e ) != 0) //Ver si conviene limitar este while con un for con limite de ciclos aparte del PollEvent
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
					break;
                }

                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
					log->info("Se selecciono ESC, juego pausado");
                    salirJuego = true;
					break;
                }

                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p) {
                    SoundManager::get_instance()->musicOn_off();
					break;
                }
                
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_o) {
					SoundManager::get_instance()->soundEffectsOn_off();
                    break;
				}

                //controlled->handleEvent(e);

				//std::cout << "Enviando un comando" << std::endl;
				commandSender.handleEvent(e);
				/*if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_q) {
					camera.follow(controlled->getEntity());
				}*/
            }

            //Cuando el tiempo pasado es mayor a nuestro tiempo de actualizacion
			if (accumulator >= fixed_dt)
			{
				//Calcula movimientos
				//std::cout << "Vamos a actualizar el modelo con lo que venga" << std::endl;
				world.update(commandSender); //Update de todos los players (y otras entidades proximamente?)
				//std::cout << "Terminamos de actualizar el modelo" << std::endl;
				camera.update(fixed_dt);
				accumulator = 0;


			}
				//Renderizamos solo despues de un update
				//std::cout << "--Renderizando--" << std::endl;

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render current frame
				//std::cout << "Renderizamos ahora" << std::endl;
				camera.render(world);

            //quit Si seleciono la tecla escape entonces pregunto si quiere salir
            if(salirJuego){
                SDL_RenderClear( gRenderer );
                if(gameMenu.pausaMenu(e)) {
                    quit = true;
                }
                salirJuego = false;
            }
            SDL_RenderPresent( gRenderer );
        }
    }

}

std::string getLogType(char *cadena) {
    std::string string(cadena);
    if( string.compare(LOG_ERROR)==0 || string.compare(LOG_DEBUG)==0 || string.compare(LOG_INFO)==0){
        return string;
    }
    return "NOT_FOUND";
}

