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
	const int SCREEN_WIDTH_SCROLL_OFFSET = SCREEN_WIDTH / 6;
	const int SCREEN_HEIGHT_SCROLL_OFFSET = SCREEN_HEIGHT / 6;

    const std::string background_path = "res/soccer_field_1.6.png";
    const std::string selectedPlayer_path = "res/player_selected.png";
    /*
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
     */

}

// YAML se traduce y se cargan las texturas en un map {sprite_id: Texture/Animation}

enum PlayerState {
    STILL = 0,
    RUNNING,
    SWEEPING,
    KICKING,
    _LENGTH_
};

enum BallState {
	MOVING=0,
	QUIESCENT=1
};

//Por cada tipo/objeto...
//Lo que puede venir dentro del YAML, pero no necesariamente
struct player_data {
     std::string sprite_ids[PlayerState::_LENGTH_]; //o directamente texturas?
    //const Animation&[Player::_LENGTH_] animation_ids;        <-
	 int widths[PlayerState::_LENGTH_];
	 int heights[PlayerState::_LENGTH_];
     int X_VELOCITY;
     int Y_VELOCITY;
     double SWEEP_DURATION;
	 double KICK_DURATION;
	 double SPRINT_VELOCITY_MULTIPLIER;
};

typedef struct player_data player_data_t;


//ejemplo instanciacion player data especifico:

