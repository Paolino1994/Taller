#ifndef __FORMATION_CREATOR_H__
#define __FORMATION_CREATOR_H__

#include <map>
#include <string>
#include "Animation.h"

#include "PlayerModel.h"
#include "PlayerView.h"
#include "PlayerController.h"
#include "World.h"
#include "Log.h"
#include <sstream>

#define LEFT_GOAL 0
#define RIGHT_GOAL 1

struct player {
	PlayerModel* model;
	PlayerView* view;
	PlayerController* controller;
};

class TeamFactory {
private:
	player_data_t BasePlayer;
	Log* log;

	std::vector<player> team;
	/**
		@PRE: recive, el lado de la cancha y las dimensiones de la cancha.
		@POS: retorna true si se pudo agregar un arquero al equipo, false si no
	*/
	bool add_goalkeeper(int goal, int field_length, int field_width);
	/**
		@PRE: recive la cantidad de defensores, el lado de la cancha y las dimensiones de la cancha.
		@POS: retorna true si se pudo agregar los defensores al equipo, false si no
	*/
	bool add_defenders(int quantity, int goal, int field_length, int field_width);
	/**
		@PRE: recive la cantidad de mediocampistas, el lado de la cancha y las dimensiones de la cancha.
		@POS: retorna true si se pudo agregar los mediocampistas al equipo, false si no
	*/
	bool add_midfielders(int quantity, int goal, int field_length, int field_width);
	/**
		@PRE: recive la cantidad de delanteros, el lado de la cancha y las dimensiones de la cancha.
		@POS: retorna true si se pudo agregar los delanteros al equipo, false si no
	*/
	bool add_forwards(int quantity, int goal, int field_length, int field_width);

	void clear_team();

public:
	/**
		@PRE: recive la formacion, el lado de la cancha y las dimensiones de la cancha.
		@POS: retorna true si se pudo crear el equipo, false si no
	*/
	bool create(int defenders, int midfielders, int forwards, int goal, int field_length, int field_width);
	/**
		@PRE: recive una formacion sin mediocampistas, el lado de la cancha y las dimensiones de la cancha.
		@POS: retorna true si se pudo crear el equipo, false si no
	*/
	bool create(int defenders, int midfielders, int goal, int field_length, int field_width);
	/**
		@PRE: recive la animacion de los jugadores
		@POS: se le agrego dicha animacion a todos los jugadores y se les ligo un controlador
	*/
	void add_view(std::map<const std::string, Animation> animMapper);
	/**
		@PRE: recive el mundo
		@POS: se le agrego al mundo todos los jugadores en el equipo
	*/
	bool add_to_world(World& world);
	/**
		@POS: retorna el equipo en el estado que esta, si no se creo estara vacio
	*/
	std::vector<player>& get_team();

	TeamFactory(player_data_t defaultPlayer);

	~TeamFactory();
};

#endif
