#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>
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
#include "common/Log.h"
#include "PlayerModel.h"
#include "PlayerView.h"
#include "PlayerControllerHuman.h"
#include "TeamFactory.h"
#include "YAMLReader.h"
#include "Texto.h"
#include "TextureSetter.h"
#include "BallModel.h"
#include "BallView.h"
#include "BallController.h"

//Screen dimension constants
const int SCREEN_WIDTH = YAML::SCREEN_WIDTH;
const int SCREEN_HEIGHT = YAML::SCREEN_HEIGHT;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

void setPlayerRun(YAMLReader reader);

std::string getLogType(char *string);

void renderizar(std::vector<player>::iterator iterator, TeamFactory *pFactory, Camera camera, World world, Texto texto,
                PlayerControllerHuman *pHuman);

player_data_t
crearDefaultPlayer(sprite_info PlayerStill, sprite_info PlayerRun, sprite_info PlayerSweep, sprite_info PlayerKick);


void addBallToWorld(World& world);

//Starts up SDL and creates window
bool init_SDL()
{
    //Initialization flag
    bool success = true;
	Log* log = Log::get_instance();
	std::stringstream msg;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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
        gWindow = SDL_CreateWindow( "TEHKAN FIUBA CUP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
}

typedef std::chrono::steady_clock Clock;

int main( int argc, char* args[] )
{
    std::string yamlConfigFile = "";
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
                log->error("Log ingresado invalido. Log cargado en modo error");
            }
        } else if (strcmp(args[i],"-yaml") == 0) {
            yamlConfigFile = args[i+1];
        }
    }
    YAMLReader& yamlReader = YAMLReader::get_instance();
    yamlReader.readYamlGeneral(yamlConfigFile);
    if (!Log::is_initialized()) {
        std::string logLevel = yamlReader.getLogLevel();
        std::string logType=getLogType((char *) logLevel.c_str());
        Log::initialize(logType);
        Log* log=Log::get_instance();
        log->error("Log cargado en modo " + logLevel);
    }
    

    //yamlReader->readYamlEquipos();

	Log* log = Log::get_instance();

    //Start up SDL and create window
    if ( !init_SDL() ) {
        std::cout << "Failed to initialize!\n" << std::endl;
		log->error("Falló la inicialización del SDL");
    } else {
        std::map<const std::string, Animation> animMapper;
        std::map<const std::string, Animation> animMapper2;
        std::map<const std::string, Animation> animMapperBall;
        // CARGAR La configuracion del YAML y de constantes nuestras:
		// TODO

        //Background:
        Texture background(gRenderer, YAML::background_path);
        // Jugador Seleccionado:
        Texture selectedPlayerTecture(gRenderer, YAML::selectedPlayer_path);

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
        animMapper.emplace(std::make_pair(PlayerRun.spriteid, Animation(runT, PlayerRun)));
        log->debug("Crear Animacion Still");
        animMapper.emplace(std::make_pair(PlayerStill.spriteid, Animation(stillT, PlayerStill)));
        log->debug("Crear Animacion Sweep");
        animMapper.emplace(std::make_pair(PlayerSweep.spriteid, Animation(sweepT, PlayerSweep)));
        log->debug("Crear Animacion Kick");
        animMapper.emplace(std::make_pair(PlayerKick.spriteid, Animation(kickT, PlayerKick)));

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
        animMapper2.emplace(std::make_pair(PlayerRun2.spriteid, Animation(runT2, PlayerRun2)));
        log->debug("Crear Animacion Still");
        animMapper2.emplace(std::make_pair(PlayerStill2.spriteid, Animation(stillT2, PlayerStill2)));
        log->debug("Crear Animacion Sweep");
        animMapper2.emplace(std::make_pair(PlayerSweep2.spriteid, Animation(sweepT2, PlayerSweep2)));
        log->debug("Crear Animacion Kick");
        animMapper2.emplace(std::make_pair(PlayerKick2.spriteid, Animation(kickT2, PlayerKick2)));



        // Creo jugadores:
        player_data_t defaultPlayer=crearDefaultPlayer(PlayerStill,PlayerRun,PlayerSweep,PlayerKick);
        log->info("Crear Jugadores");
		TeamFactory* tfactory = new TeamFactory(defaultPlayer);
        int defensores=YAMLReader::get_instance().getDefensores(equipo);
        int mediocampistas=YAMLReader::get_instance().getMediocampistas(equipo);
        int delanteros=YAMLReader::get_instance().getDelanteros(equipo);
		tfactory->create(defensores, mediocampistas, delanteros, LEFT_GOAL, background.getWidth(), background.getHeight());
		tfactory->add_view(animMapper);




        // Iterador de jugadores en el switcheo
        std::vector<player>::iterator teamIterator = std::prev(tfactory->get_team().end());

        log->info("Crear Animaciones de la pelota");
        TextureSetter texturesBall(BALL, gRenderer);
        sprite_info ballStill=texturesBall.getBallStillInfo();
        Texture ballStillT=texturesBall.getBallStillTexture();
        sprite_info ballMoving=texturesBall.getBallMovingInfo();
        Texture ballMovingT=texturesBall.getBallMovingTexture();
        //sprite_info ballStill=texturesBall.getBallStillInfo();
        animMapperBall.emplace(std::make_pair(ballStill.spriteid, Animation(ballStillT, ballStill)));
        animMapperBall.emplace(std::make_pair(ballMoving.spriteid, Animation(ballMovingT, ballMoving)));

        //Crteo la pelota
        log->info("Crear Pelota");
        teamIterator->model->setHasControlOfTheBall(true);
        BallModel *ballModel=new BallModel(0, 0, teamIterator->model->getX(), teamIterator->model->getY());
        Log::get_instance()->info("Agregando vista de la pelota");
        BallView* ballView= new BallView(animMapperBall, ballModel);
        BallController::initialize(ballModel, ballView);


        //creo sprites equipo2

        //agrego equipo 2
        player_data_t defaultPlayer2=crearDefaultPlayer(PlayerStill2,PlayerRun2,PlayerSweep2,PlayerKick2);
        TeamFactory* tfactory2 = new TeamFactory(defaultPlayer2);
        log->info("Crear Jugadores del team 2");
        defensores=YAMLReader::get_instance().getDefensores(2);
        mediocampistas=YAMLReader::get_instance().getMediocampistas(2);
        delanteros=YAMLReader::get_instance().getDelanteros(2);
        tfactory2->create(defensores, mediocampistas, delanteros, RIGHT_GOAL, background.getWidth(), background.getHeight());
        tfactory2->add_view(animMapper2);



		// Inyecto un jugador controlado por un humano
        log->info("Cargar Controlladores");
		PlayerControllerHuman* controlled = new PlayerControllerHuman(teamIterator->model, teamIterator->view);
		delete teamIterator->controller;
		teamIterator->controller = controlled;
        teamIterator->model->setIsControlledByHuman(true);

        // Agrego mensaje para salir del juego
        log->info("Cargar Mensaje salida de juego");
        Texto quiereSalirTexto(gRenderer, "res/Tehkan World Cup.ttf",36, "SALIR DEL JUEGO? S/N", {255,255,0,0});

		// Agrego jugadores al mundo
        log->info("Agrego Jugadores al Juego");
        World world(background.getWidth(), background.getHeight(), &background);
        world.setPlayerSelectedTexture(&selectedPlayerTecture);
		tfactory->add_to_world(world);
        tfactory2->add_to_world(world);
        log->info("Agrego la pelota");
        addBallToWorld(world);
        log->info("Agrego la camara");

        Camera camera(world, SCREEN_WIDTH, SCREEN_HEIGHT, YAML::SCREEN_WIDTH_SCROLL_OFFSET, YAML::SCREEN_HEIGHT_SCROLL_OFFSET);
        camera.follow(teamIterator->model);
        log->info("Renderizo");
        renderizar(teamIterator, tfactory, camera, world,
                   quiereSalirTexto, controlled);
		
		//Free team resources
		delete tfactory;

    }

    //Free resources and close SDL
    close();

    return 0;
}

void addBallToWorld(World& world) {
    BallController* cont=BallController::getInstance();
    BallModel* mod=cont->getModel();
    world.addEntity(mod);
    //std::cout<<"HOLA"<<std::endl;
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

void
renderizar(std::vector<player>::iterator teamIterator, TeamFactory *tfactory, Camera camera, World world,
           Texto quiereSalirTexto, PlayerControllerHuman *controlled) {
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
        bool salirJuego = false;

		Log* log = Log::get_instance();

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

                if (e.type == SDL_KEYDOWN ){
                    if(e.key.keysym.sym == SDLK_q) {
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
                    if ( e.key.keysym.sym == SDLK_ESCAPE) {
                        log->info("Se selecciono ESC, juego pausado");
                        salirJuego = true;
                    }
                }


                controlled->handleEvent(e);
            }

            //Cuando el tiempo pasado es mayor a nuestro tiempo de actualizacion
            while ( accumulator >= fixed_dt )
            {
                //Calcula movimientos
                world.update(fixed_dt); //Update de todos los players (y otras entidades proximamente?)
                camera.update(fixed_dt);
                accumulator -= fixed_dt;
            }

            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            //Render current frame
            camera.render(world);


            // Si seleciono la tecla escape entonces pregunto si quiere salir
            if(salirJuego){
                int w, h;
                quiereSalirTexto.getTextureDimensions(&w,&h); // pregunto el tamanio para el centrado
                quiereSalirTexto.display((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2); // muestro la pregunta centrada
                SDL_RenderPresent( gRenderer ); // renderizo la pantalla con la pregunta
                while(salirJuego && SDL_WaitEvent(&e) != 0){ // mientras no haya seleccionado s o n el juego esta parado
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s) { // Con s sale del juego
                        quit = true;
                        salirJuego = false;
						log->info("SALIENDO DEL JUEGO");
                    }
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_n) { // Con n vuelve al juego
						log->info("Salida del juego cancelada");
                        salirJuego = false;
                    }
                }
            } else {
                //Update screen
                SDL_RenderPresent( gRenderer );
            }
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

