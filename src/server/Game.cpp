#include "Game.h"

#include <iostream>

#include "common/EventQueue.h"
#include "common/GameConstants.h"
#include "common/Log.h"
#include "common/SpriteInfoSetter.h"
#include "../common/GameConstants.h"

#include "BallPlayerCollisionSystem.h"
#include "BallPassesEndLineSystem.h"
#include "TimerSystem.h"

#include "GameManager.h"
#include "common/Protocol.h"

player_data_t crearDefaultPlayer(sprite_info PlayerStill, sprite_info PlayerRun, sprite_info PlayerSweep, sprite_info PlayerKick) {
	player_data_t defaultPlayer = { {
			[PlayerState::STILL] = PlayerStill.spriteid,
			[PlayerState::RUNNING] = PlayerRun.spriteid,
			[PlayerState::SWEEPING] = PlayerSweep.spriteid,
			[PlayerState::KICKING] = PlayerKick.spriteid,

		},

		/*WIDTHS*/{
			[PlayerState::STILL] = PlayerStill.width,
			[PlayerState::RUNNING] = PlayerRun.width,
			[PlayerState::SWEEPING] = PlayerSweep.width,
			[PlayerState::KICKING] = PlayerKick.width,
		},
		/*HEIGHTS*/{
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

std::map<const std::string, Animation> getAnimMapperBall() {
	std::map<const std::string, Animation> animMapperBall;
	Log::get_instance()->info("Crear Animaciones de la pelota");
	SpriteInfoSetter texturesBall(BALL);
	sprite_info ballStill = texturesBall.getBallStillInfo();
	sprite_info ballMoving = texturesBall.getBallMovingInfo();
	//sprite_info ballStill=texturesBall.getBallStillInfo();
	animMapperBall.emplace(std::make_pair(ballStill.spriteid, Animation(ballStill)));
	animMapperBall.emplace(std::make_pair(ballMoving.spriteid, Animation(ballMoving)));
	return animMapperBall;
}

void Game::_run()
{
	using Clock = std::chrono::steady_clock;
	//El tema del clock, es para actualizar en intervalos fijos
	//Y renderizar en el resto del tiempo
	Clock::time_point currentTime, newTime;
	currentTime = Clock::now();
	std::chrono::milliseconds milli;
	const double fixed_dt = 0.01; //10 milliseconds
	double accumulator = 0; // en segundos!
	double frametime;

    //Log* log = Log::get_instance();

	//While application is running
	while (!server_exit_requested)
	{
		newTime = Clock::now();
		milli = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime);
		currentTime = newTime;
		frametime = milli.count() / 1000.0;
        accumulator += frametime;
        //time_t end = time(NULL);
        /*if((double)(end-lastime)>0){
            std::cout<<"Execution Time: "<< (double)(end-start)<<" Seconds"<<std::endl;
            lastime=end;
        }*/

		// Render current frame
		// TODO: Aca enviar el modelo a nuestos clientes
		// Ojo: Enviar el modelo solo cuando cambio --> hacer algun tipo de sleep
		//modelData.playerViewData.clear();
		//world.serialize(modelData);

		//int64_t sleep = (fixed_dt - accumulator) * 1000;
		//std::cout << "Sleeping for " << sleep << " millis" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5)); // fixed_dt en millisegundos

		// Handle events on queue
		// Por ahora no usamos event queue -> Permitimos que los controllers desde los RequestHandlers le pegen directo al modelo
		// TODO falta mutexear modelo para que en update no se puedan llamar metodos a los models

		//Cuando el tiempo pasado es mayor a nuestro tiempo de actualizacion

		while (accumulator >= fixed_dt)
		{
			//Calcula movimientos
			//std::cout << "Model update" << std::endl;
			world.update(fixed_dt); //Update de todos los players (y otras entidades proximamente?)

			EventQueue::get().handleEvents();
			accumulator -= fixed_dt;
			modelData.playerViewData.clear();
            //modelData.timeInSeconds=end-start;
			//std::cout << "Model serialize" << std::endl;
			world.serialize(modelData);
		}
	}

}

Game::Game() :
        playerViewData(std::vector<player_view_data_t>()),
        ballViewData({ 0,0,0,0,QUIESCENT }),
        gameManagerData({0,0,0}),
        modelData({ playerViewData, ballViewData, gameManagerData}),
        world(World(YAML::WORLD_WIDTH, YAML::WORLD_HEIGHT, getAnimMapperBall())),
        maxPlayers(YAML::MAX_PLAYERS),
        playerCount(0),
        running(false),
		isSetFormationHome(false),
		isSetFormationAway(false),
        server_exit_requested(false)
{
    /****************************************
    ** INICIO CREACION TEXTURAS Y ANIMACIONES
    */

    std::map<const std::string, Animation> animMapper;
    std::map<const std::string, Animation> animMapper2;

    // CARGAR La configuracion del YAML y de constantes nuestras:
    // TODO

    Log* log = Log::get_instance();

    //Las texturas:
    log->info("Cargando Texturas");

    // EQUIPO 1 HAY QUE MODULARIZAR ESTO
    int equipo = 1;
    log->info("Crear Texturas");
    SpriteInfoSetter textures(equipo); // No me gusta esto del lado del servidor
    sprite_info PlayerRun = textures.getPlayerRunInfo();
    sprite_info PlayerStill = textures.getPlayerStillInfo();
    sprite_info PlayerSweep = textures.getPlayerSweepInfo();
    sprite_info PlayerKick = textures.getPlayerKickInfo();

    // Crear animaciones en base a datos del sprite y mandarlos a un map para el Player
    log->info("Crear Animaciones");
    log->debug("Crear Animacion Run");
    animMapper.emplace(std::make_pair(PlayerRun.spriteid, Animation(PlayerRun)));
    log->debug("Crear Animacion Still");
    animMapper.emplace(std::make_pair(PlayerStill.spriteid, Animation(PlayerStill)));
    log->debug("Crear Animacion Sweep");
    animMapper.emplace(std::make_pair(PlayerSweep.spriteid, Animation(PlayerSweep)));
    log->debug("Crear Animacion Kick");
    animMapper.emplace(std::make_pair(PlayerKick.spriteid, Animation(PlayerKick)));

    SpriteInfoSetter textures2(2);
    sprite_info PlayerRun2 = textures2.getPlayerRunInfo();
    sprite_info PlayerStill2 = textures2.getPlayerStillInfo();
    sprite_info PlayerSweep2 = textures2.getPlayerSweepInfo();
    sprite_info PlayerKick2 = textures2.getPlayerKickInfo();
    // Crear animaciones en base a datos del sprite y mandarlos a un map para el Player
    log->info("Crear Animaciones del equipo 2");
    log->debug("Crear Animacion Run");
    animMapper2.emplace(std::make_pair(PlayerRun2.spriteid, Animation(PlayerRun2)));
    log->debug("Crear Animacion Still");
    animMapper2.emplace(std::make_pair(PlayerStill2.spriteid, Animation(PlayerStill2)));
    log->debug("Crear Animacion Sweep");
    animMapper2.emplace(std::make_pair(PlayerSweep2.spriteid, Animation(PlayerSweep2)));
    log->debug("Crear Animacion Kick");
    animMapper2.emplace(std::make_pair(PlayerKick2.spriteid, Animation(PlayerKick2)));



    /*
    ** FIN CREACION TEXTURAS Y ANIMACIONES
    **************************************/

    // Creo jugadores:
    player_data_t defaultPlayer = crearDefaultPlayer(PlayerStill, PlayerRun, PlayerSweep, PlayerKick);
    log->info("Crear Jugadores");
    //TeamFactory* tfactory = new TeamFactory(defaultPlayer);
    int defensores = YAMLReader::get_instance().getDefensores(equipo);
    int mediocampistas = YAMLReader::get_instance().getMediocampistas(equipo);
    int delanteros = YAMLReader::get_instance().getDelanteros(equipo);

    world.createTeam(Team::HOME, defensores, mediocampistas, delanteros, defaultPlayer, animMapper);

	world.setSetPiecePosition(Team::HOME, FIELD_POSITION::LEFT, SET_PIECE::KICKOFF); // ACA estoy emprolijando la formacion para el saque inicial


    //agrego equipo 2
    player_data_t defaultPlayer2 = crearDefaultPlayer(PlayerStill2, PlayerRun2, PlayerSweep2, PlayerKick2);
    //TeamFactory* tfactory2 = new TeamFactory(defaultPlayer2);
    log->info("Crear Jugadores del team 2");
    defensores = YAMLReader::get_instance().getDefensores(2);
    mediocampistas = YAMLReader::get_instance().getMediocampistas(2);
    delanteros = YAMLReader::get_instance().getDelanteros(2);

    world.createTeam(Team::AWAY, defensores, mediocampistas, delanteros, defaultPlayer2, animMapper2);

	world.setSetPiecePosition(Team::AWAY, FIELD_POSITION::RIGHT, SET_PIECE::KICKOFF); // ACA estoy emprolijando la formacion para el saque inicial

	// PUEDE SER importante el orden de agregado de sistemas
	world.addSystem(std::make_shared<TimerSystem>(10 * 10, true)); // tiempos de 1 minuto emulados a 45 min
    world.addSystem(std::make_shared<BallPlayerCollisionSystem>(world));
    world.addSystem(std::make_shared<BallPassesEndLineSystem>(world));

    world.serialize(this->modelData);
}



Game::~Game()
{
	if (this->running) { // me estan terminando desde afuera, es decir el servidor quiere cerrar
		this->server_exit_requested = true;
		// Lo haria el main esto: > for protocol in procols -> protocol.shutdown(); //deberia desbloquear el thread
		this->worker.join();
	}
}

PlayerController * Game::assignToTeam(Team team, User_ID userId)
{
	this->playerCount++;
	if (this->playerCount == maxPlayers) {
		if (!running) {
			worker = std::thread(&Game::_run, this);
			running = true;
		}
	}

	PlayerController* controller= this->world.injectHumanController(team, userId);
	if(playerCount==1){
		controller->getModel()->setHasControlOfTheBall(true);
		GameManager::get_instance().setLastBallControlUser(controller->getUserId());
	}
	return controller;
}


bool Game::teamFormation(Team team, User_ID userId)
{
	if(team == Team::HOME  && isSetFormationHome){
		std::cout << "HOME" << std::endl;
		return true;
	}
	if(team == Team::AWAY  && isSetFormationAway){
		std::cout << "AWAY" << std::endl;
			return true;
	}
	std::cout << "FALSE" << std::endl;
	return false;
}

void Game::assignToFormation(Team team, Formation formation, User_ID userId)
{

	FIELD_POSITION position = FIELD_POSITION::__LENGTH__;
	if(team == Team::HOME){
		isSetFormationHome = true;
		position = FIELD_POSITION::LEFT;
	}else{
		isSetFormationAway = true;
		position = FIELD_POSITION::RIGHT;
	}

	std::string textFormation = "";
	if(formation == Formation::FORMATION_1){
		textFormation = "3-2-1";
	}else{
		if(formation == Formation::FORMATION_2){
			textFormation = "3-1-2";
		}else
			textFormation = "3-3-0";
	}

	this->world.changeFormation(team, position, textFormation);
}

model_data_t Game::getModelData()
{
	return modelData;
}

bool Game::withdrawUser(PlayerController * playerController, User_ID userId)
{
	return this->world.ejectController(playerController, userId);
}
