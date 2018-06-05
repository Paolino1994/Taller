#pragma once
#include <string>
#include "common/YAMLReader.h"
#include "SDL_rect.h"

using Player_ID = u_int32_t;
using User_ID = int32_t;
const User_ID AI_USER = -1;
using pos_t = int32_t;
using angle_t = int32_t;
using frame_t = u_int32_t;

enum class Team : u_int32_t {
	HOME = 0, //LEFT_GOAL
	AWAY, //RIGHT_GOAL
	__LENGTH__
};

typedef struct sprite_info {
    std::string spriteid;
    std::string file_path;
    int width; //escalado a destino(pantalla)
    int height; //escalado a destino(pantalla)
    int frames; // para Animation
    int frames_per_second; // para Animation
} sprite_info_t;

enum FIELD_POSITION {
		LEFT = 0,
		RIGHT,
		__LENGTH__
	};


// Datos que pueden venir de la configuracion
namespace YAML {
	// Identicos a nuestra imagen de background en el cliente
	// Si lo quieren manejar por separado hay que aplicar logica de escalado del mundo logico al background -> no es la idea
	const int WORLD_WIDTH = 1700;
	const int WORLD_HEIGHT = 950;

	// Por ahora lo pensamos como un SDL_Rect //esquina superior izquierda
	// pero puede cambiar
	const int FIELD_X = 62; 
	const int FIELD_Y = 44;
	const int FIELD_WIDTH = 1574;
	const int FIELD_HEIGHT = 968;

	const int FIELD_CENTER_X = 850;
	const int FIELD_CENTER_Y = 530;

	// Por ahora lo pensamos como un SDL_Rect //esquina superior izquierda
	// pero puede cambiar
	const SDL_Rect GOAL[FIELD_POSITION::__LENGTH__] = {
		{1637, 432, 63, 198}, // {x,y,w,h}
		{0, 432, 61, 198 }
	};

	const size_t MAX_PLAYERS = 1; //TODO: Esto recibirlo desde el YAMLReader

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const int MINIMAP_HEIGHT = 200;
    const int MINIMAP_WIDTH = 250;
    const int MINIMAP_INIT_X = SCREEN_WIDTH / 2 - MINIMAP_WIDTH / 2;
	const int SCREEN_WIDTH_SCROLL_OFFSET = SCREEN_WIDTH / 2;
	const int SCREEN_HEIGHT_SCROLL_OFFSET = SCREEN_HEIGHT / 2;

    const std::string background_path = "res/soccer_field_1.6.png";
    const std::string indicators_path = "res/indicators";
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

typedef struct player_view_data {
	User_ID userId;
	Player_ID playerId;
	Team team;
	pos_t x;
	pos_t y;
	angle_t angle;
	PlayerState state;
	frame_t presentFrame;
} player_view_data_t;

typedef struct ball_view_data {
	pos_t x;
	pos_t y;
	pos_t z;
	angle_t angle;
	BallState state;
	frame_t presentFrame;
} ball_view_data_t;

typedef struct game_manager_data {
	int scoreHome;
	int scoreAway;
    uint32_t timeInSeconds;
    FIELD_POSITION homeDefends;
    FIELD_POSITION awayDefends;
    int period;
} game_manager_data_t;


enum class EventID : uint32_t {
	KICK = 0,
	PERIOD_END,
	__LENGTH__
};

typedef struct model_data {
	std::vector<player_view_data_t>& playerViewData;
	ball_view_data_t& ballViewData;
    game_manager_data_t& gameManagerData;
	std::vector<EventID>& events; // probablemente cambie

} model_data_t;
