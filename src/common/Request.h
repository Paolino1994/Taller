#pragma once

#include <arpa/inet.h>

/*
*	Todo tipo de dato a enviar por la red, especificarlo con los tipos fijos, por las dudas
*/

enum class Request : u_int32_t {
	NONE = 0,
	TEAM_ASSIGN,
	COMMAND,
	PLAYER_VIEW_UPDATE,
	BALL_VIEW_UPDATE,
	LOGIN,
	START
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
