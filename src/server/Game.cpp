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
#include "RestartSystem.h"

#include "GameManager.h"

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

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		//Cuando el tiempo pasado es mayor a nuestro tiempo de actualizacion
		while (accumulator >= fixed_dt)
		{		
			// Bloqueamos el modelo hasta el fin del update y serializacion
			std::unique_lock<std::mutex> updateLock(this->updateMtx);
			world.update(fixed_dt);

			EventQueue::get().handleEvents();
			accumulator -= fixed_dt;

			// Recien aca bloqueamos para serializar
			std::unique_lock<std::mutex> serializeLock(this->serializeMtx);
			modelData.playerViewData.clear();
            world.serialize(modelData);
			this->modelSnapshotNumber++;
		}
	}

}

Game::Game() :
	modelData({ std::vector<player_view_data_t>() ,{ 0,0,0, 0, QUIESCENT, 0 },{ 0,0,0, FIELD_POSITION::LEFT, FIELD_POSITION::RIGHT, 0, 0, 0, 0, 0, 0 } }),
	modelSnapshotNumber(0),
    world(World(YAML::WORLD_WIDTH, YAML::WORLD_HEIGHT, getAnimMapperBall())),
    maxPlayers(YAML::MAX_PLAYERS),
    playerCount(0),
    running(false),
	existFormationHome(false),
	existFormationAway(false),
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
	world.setZonesDistances(Team::HOME);

    //agrego equipo 2
    player_data_t defaultPlayer2 = crearDefaultPlayer(PlayerStill2, PlayerRun2, PlayerSweep2, PlayerKick2);
    //TeamFactory* tfactory2 = new TeamFactory(defaultPlayer2);
    log->info("Crear Jugadores del team 2");
    defensores = YAMLReader::get_instance().getDefensores(2);
    mediocampistas = YAMLReader::get_instance().getMediocampistas(2);
    delanteros = YAMLReader::get_instance().getDelanteros(2);

    world.createTeam(Team::AWAY, defensores, mediocampistas, delanteros, defaultPlayer2, animMapper2);

	world.setSetPiecePosition(Team::AWAY, FIELD_POSITION::RIGHT, SET_PIECE::KICKOFF); // ACA estoy emprolijando la formacion para el saque inicial
	world.setZonesDistances(Team::AWAY);

	// PUEDE SER importante el orden de agregado de sistemas
	world.addSystem(std::make_shared<TimerSystem>(1 * 10, true)); // tiempos de 1 minuto emulados a 45 min
    world.addSystem(std::make_shared<BallPlayerCollisionSystem>(world));
    world.addSystem(std::make_shared<BallPassesEndLineSystem>(world));
	world.addSystem(std::make_shared<RestartSystem>(10)); //cuando termina el juego, lo reinicia en 10 segundos

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
	std::unique_lock<std::mutex> updateLock(this->updateMtx);

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


bool Game::checkTeamFormation(Team team, User_ID userId)
{
	std::unique_lock<std::mutex> teamFormationLock(this->teamFormationMtx);

	return ((team == Team::HOME && existFormationHome) || (team == Team::AWAY && existFormationAway));
}

void Game::setTeamFormation(Team team, Formation formation, User_ID userId)
{
	std::unique_lock<std::mutex> teamFormationLock(this->teamFormationMtx);

	FIELD_POSITION position = FIELD_POSITION::__LENGTH__;

	if(team == Team::HOME){
		existFormationHome = true;
		position = FIELD_POSITION::LEFT;
	}else{
		existFormationAway = true;
		position = FIELD_POSITION::RIGHT;
	}

	std::string textFormation = "";
	if(formation == Formation::FORMATION_3_2_1) textFormation = "3-2-1";
		else{
			if(formation == Formation::FORMATION_3_1_2) textFormation = "3-1-2";
				else
					textFormation = "3-3-0";
		}

	std::cout << "formacion de equipo: " << textFormation << std::endl;

	this->world.changeFormation(team, position, textFormation);
}

size_t Game::getModelSnapshotNumber()
{
	return this->modelSnapshotNumber;
}

void Game::copyModelData(model_data_t& modelDataReceiver)
{
	std::unique_lock<std::mutex> serializeLock(this->serializeMtx);
	modelDataReceiver.playerViewData = this->modelData.playerViewData;
	modelDataReceiver.ballViewData = this->modelData.ballViewData;
	modelDataReceiver.gameManagerData = this->modelData.gameManagerData;
}

bool Game::withdrawUser(PlayerController * playerController, User_ID userId)
{
	std::unique_lock<std::mutex> updateLock(this->updateMtx);
	return this->world.ejectController(playerController, userId);
}

void Game::handleCommandForPlayer(PlayerController * player, Command & command)
{
	std::unique_lock<std::mutex> updateLock(this->updateMtx);
	player->handleEvent(command);
}
