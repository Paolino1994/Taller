#pragma once

#include <arpa/inet.h>
#include "Request.h"

/*
*	Todo tipo de dato a enviar por la red, especificarlo con los tipos fijos, por las dudas
*/

enum class Request : u_int32_t {
	NONE = 0,
	TEAM_ASSIGN,
	COMMAND,
	PLAYER_VIEW_UPDATE,
	BALL_VIEW_UPDATE,
	GAME_MANAGER_UPDATE,
	EVENT_UPDATE,
	LOGIN,
	START,
	TEAM_FORMATION,
	TEAM_NO_FORMATION,
	SET_FORMATION,
};


// Probablemente mover a otro archivo todo lo del command
enum class CommandType : u_int32_t {
	GO_LEFT = 0,
	GO_UP,
	GO_DOWN,
	GO_RIGHT,
	SPRINT,
	KICK,
	SWEEP,
	PASS,
	CHANGE_PLAYER,
	LONG_PASS,
	__LENGTH__
};

enum class CommandKey : u_int32_t {
	KEY_DOWN = 0,
	KEY_UP,
	__LENGTH__
};

struct Command {
	CommandKey key;
	CommandType type;
};

enum class CommandTeam : u_int32_t {
	HOME = 0,
	AWAY,
	__LENGTH__
};

enum class CommandFormation : u_int32_t {
	FORMATION_1 = 0, 	//3-3-0
	FORMATION_2, 		//3-3-0
	FORMATION_3, 		//3-1-2
	__LENGTH__
};

struct CommandSetFormation {
	CommandTeam team;
	CommandFormation formation;
};

const std::string commandTypeDescription[static_cast<std::underlying_type<CommandType>::type>(CommandType::__LENGTH__)] = {
	/*[Command::GO_LEFT] =*/ "Ir a la izquierda",
	/*[Command::GO_UP] =*/ "Ir para arriba",
	/*[Command::GO_DOWN] =*/ "Ir para abajo",
	/*[Command::GO_RIGHT] =*/ "Ir a la derecha",
	/*[Command::SPRINT] =*/ "Correr",
	/*[Command::KICK] =*/ "Patear",
	/*[Command::SWEEP] =*/ "Barrer",
	/*[Command::PASS] =*/ "Pasar la pelota",
	/*[Command::CHANGE_PLAYER] =*/ "Cambiar de jugador controlado"
};
