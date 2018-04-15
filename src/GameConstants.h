#pragma once
#include <string>
#include "YAMLReader.h"

typedef struct sprite_info {
    std::string spriteid;
    std::string file_path;
    int width; //escalado a destino(pantalla)
    int height; //escalado a destino(pantalla)
    int frames; // para Animation
    int frames_per_second; // para Animation
} sprite_info_t;

// Datos que pueden venir de la configuracion
namespace YAML {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    
    const std::string background_path = "res/fondo.jpg";
    
    const sprite_info_t PlayerRun = {
        "run",
        YAMLReader::get_instance()->getSpriteRunning(EQUIPO1),
        60,
        64,
        4,
        12
    };
    
    const sprite_info_t PlayerStill = {
        "still",
		YAMLReader::get_instance()->getSpriteStill(EQUIPO1),
        68,
        34,
        1,
        3
    };

// TODO-buscar un mejor sprite
    const sprite_info_t PlayerSweep = {
        "sweep",
        YAMLReader::get_instance()->getSpriteSweeping(EQUIPO1),
        60,
        64,
        4,
        12
    };

    const sprite_info_t PlayerKick = {
        "kick",
        YAMLReader::get_instance()->getSpriteKicking(EQUIPO1),
        60,
        64,
        4,
        12
    };

}

// YAML se traduce y se cargan las texturas en un map {sprite_id: Texture/Animation}

enum PlayerState {
    STILL = 0,
    RUNNING,
    SWEEPING,
    KICKING,
    _LENGTH_
};

//Por cada tipo/objeto...
//Lo que puede venir dentro del YAML, pero no necesariamente
struct player_data {
    const std::string sprite_ids[PlayerState::_LENGTH_]; //o directamente texturas?
    //const Animation&[Player::_LENGTH_] animation_ids;        <-
    const int X_VELOCITY;
    const int Y_VELOCITY;
    const double SWEEP_DURATION;
	const double KICK_DURATION;
	const double SPRINT_VELOCITY_MULTIPLIER;
};

typedef struct player_data player_data_t;


//ejemplo instanciacion player data especifico:
const player_data_t DEFAULT_PLAYER = {

	// Asignar los estados a sus respectivos sprites
	// sprite_ids[Player::State] -> sprite_id
	/*sprite_ids =*/ { //usando designators
		[PlayerState::STILL] = YAML::PlayerStill.spriteid,
		[PlayerState::RUNNING] = YAML::PlayerRun.spriteid,
		[PlayerState::SWEEPING] = YAML::PlayerSweep.spriteid,
        [PlayerState::KICKING] = YAML::PlayerKick.spriteid,

	},

	// pixeles (logicos) por segundo
	/*X_VELOCITY =*/ 200,
	/*Y_VELOCITY =*/ 200,
    /*SWEEP_DURATION =*/ (1.0 / YAML::PlayerSweep.frames_per_second) * YAML::PlayerSweep.frames,
    /*KICK_DURATION =*/ (1.0 / YAML::PlayerKick.frames_per_second) * YAML::PlayerKick.frames,
	/*SPRINT_VEL_MULT*/ 1.5
};
