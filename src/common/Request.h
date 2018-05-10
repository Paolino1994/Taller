#pragma once

#include <arpa/inet.h>

/*
*	Todo tipo de dato a enviar por la red, especificarlo con los tipos fijos, por las dudas
*/

enum class Request : u_int32_t {
	NONE = 0,
	COMMAND_KEY_DOWN,
	COMMAND_KEY_UP
};


// Probablemente mover a otro archivo todo lo del command
enum class Command : u_int32_t {
	GO_LEFT = 0,
	GO_UP,
	GO_DOWN,
	GO_RIGHT,
	SPRINT,
	KICK,
	SWEEP,
	CHANGE_PLAYER,
	__LENGTH__
};

const std::string commandDescription[static_cast<std::underlying_type<Command>::type>(Command::__LENGTH__)] = {
	/*[Command::GO_LEFT] =*/ "Ir a la izquierda",
	/*[Command::GO_UP] =*/ "Ir para arriba",
	/*[Command::GO_DOWN] =*/ "Ir para abajo",
	/*[Command::GO_RIGHT] =*/ "Ir a la derecha",
	/*[Command::SPRINT] =*/ "Correr",
	/*[Command::KICK] =*/ "Patear",
	/*[Command::SWEEP] =*/ "Barrer",
	/*[Command::CHANGE_PLAYER] =*/ "Cambiar de jugador controlado"
};
