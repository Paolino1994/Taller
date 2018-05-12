#include "TeamFactory.h"

#include "PlayerControllerAI.h"


TeamFactory::TeamFactory(player_data_t defaultPlayer){
	BasePlayer=defaultPlayer;
	this->log = Log::get_instance();
}
bool TeamFactory::add_goalkeeper(Team goal, int field_length, int field_width){
	PlayerModel* model;

	switch (goal) {
		case LEFT_GOAL:
			model = new PlayerModel(goal, BasePlayer, field_length * 1.5 / 22, field_width * 21/ 44);
			break;
		case RIGHT_GOAL:
			model = new PlayerModel(goal, BasePlayer, field_length * 20.5 / 22, field_width * 21 / 44);
			break;
		default:
			std::stringstream msg;
			msg << "TeamFactory: el arco " << static_cast<std::underlying_type<Team>::type>(goal) << " es invalido";
			log->error(msg.str());
			return false;
	}

	player goalkeeper;
	goalkeeper.model = model;
	team.push_back(goalkeeper);
	return true;
}


bool TeamFactory::add_defenders(int quantity, Team goal, int field_length, int field_width){
	if (quantity != 3) {
		std::stringstream msg;
		msg << "TeamFactory: la cantidad de defensores " << quantity << " es invalida"; 
		log->error(msg.str());
		return false;
	}

	int position_x, position_y;

	switch (goal) {
		case LEFT_GOAL:
			position_x = field_length * 6 / 22;
			break;
		case RIGHT_GOAL:
			position_x = field_length * 16 / 22;
			break;
		default:
			std::stringstream msg;
			msg << "TeamFactory: El arco " << static_cast<std::underlying_type<Team>::type>(goal) << " es invalido";
			log->error(msg.str());
			return false;
	}

	PlayerModel* model;
	player defender;

	for (int i = 0; i < quantity; i++){
		position_y = ((field_width * 5 / 8) * i / 2) + 	field_width / 7;
		model = new PlayerModel(goal, BasePlayer, position_x, position_y, position_x, position_y);
		defender.model = model;
		team.push_back(defender);
	}
	return true;
}

bool TeamFactory::add_midfielders(int quantity, Team goal, int field_length, int field_width){
	if (quantity <= 0 || quantity > 3) {
		std::stringstream msg;
		msg << "TeamFactory: la cantidad de mediocampistas " << quantity << " es invalida"; 
		log->error(msg.str());
		return false;
	}

	int position_x, position_y, kickOff_x;

	switch (goal) {
		case LEFT_GOAL:
			position_x = field_length * 10 / 22;
			kickOff_x = position_x - 100;
			break;
		case RIGHT_GOAL:
			position_x = field_length * 12 / 22;
			kickOff_x = position_x + 100;
			break;
		default: 
			std::stringstream msg;
			msg << "TeamFactory: El arco " << static_cast<std::underlying_type<Team>::type>(goal) << " es invalido";
			log->error(msg.str());
			return false;
	}

	PlayerModel* model;
	player midfielder;

	for (int i = 0; i < quantity; i++){
		switch (quantity) {
			case 1:
				position_y = field_width / 2;
				break;
			case 2:
				position_y = ((field_width * 5 / 8) * (2 + 2 * i) / 5) + field_width / 8;
				break;
			case 3:
				position_y = (field_width * 5 / 8) * i / 2 + field_width / 7;
				break;
			default:
				return false;
		}
		model = new PlayerModel(goal, BasePlayer, position_x, position_y, kickOff_x, position_y);
		midfielder.model = model;
		team.push_back(midfielder);
	}

	return true;
}

bool TeamFactory::add_forwards(int quantity, Team goal, int field_length, int field_width){
	if (quantity <= 0 || quantity > 2) {
		std::stringstream msg;
		msg << "TeamFactory: la cantidad de delanteros " << quantity << " es invalida"; 
		log->error(msg.str());
		return false;
	}

	int position_x, position_y, kickOff_x, kickOff_y;

	switch (goal) {
		case LEFT_GOAL:
			position_x = field_length * 15 / 22;
			kickOff_x = field_length / 2 - 50;
			break;
		case RIGHT_GOAL:
			position_x = field_length * 7 / 22;
			kickOff_x = field_length / 2 + 50;
			break;
		default:
			std::stringstream msg;
			msg << "TeamFactory: El arco " << static_cast<std::underlying_type<Team>::type>(goal) << " es invalido";
			log->error(msg.str());
			return false;
	}

	PlayerModel* model;
	player forward;

	for (int i = 0; i < quantity; i++){
		switch (quantity) {
			case 1:
				position_y = field_width / 2;
				if(goal == LEFT_GOAL) {
					kickOff_y = position_y;
					kickOff_x = field_length / 2;
				} else {
					kickOff_y = position_y;
				}
				break;
			case 2:
				position_y = ((field_width * 5 / 8) * (1 + 4 * i) / 5) + field_width / 8;
				if(goal == LEFT_GOAL && i == quantity - 1) {
					kickOff_y = field_width / 2;
					kickOff_x = field_length / 2;
				} else {
					kickOff_y = position_y;
				}
				break;
			default:
				return false;
		}
		model = new PlayerModel(goal, BasePlayer, position_x, position_y, kickOff_x, kickOff_y);
		forward.model = model;
		team.push_back(forward);
	}

	return true;
}

bool TeamFactory::create(int defenders, int midfielders, int forwards, Team goal, int field_length, int field_width){
	clear_team();
	
	std::stringstream msg;
	msg << "TeamFactory: creando equipo con formacion "
		<< defenders
		<< "-" << midfielders
		<< "-" << forwards
		<< " en arco " << static_cast<std::underlying_type<Team>::type>(goal);
	log->info(msg.str());

	if(!(add_goalkeeper(goal, field_length, field_width)) ||
		!(add_defenders(defenders, goal, field_length, field_width)) ||
		!(add_midfielders(midfielders, goal, field_length, field_width)) ||
		!(add_forwards(forwards, goal, field_length, field_width)))
		return false;
	return true;
}

bool TeamFactory::create(int defenders, int midfielders, Team goal, int field_length, int field_width){
	clear_team();

	std::stringstream msg;
	msg << "TeamFactory: creando equipo con formacion "
		<< defenders
		<< "-" << midfielders
		<< " en arco " << static_cast<std::underlying_type<Team>::type>(goal);
	log->info(msg.str());

	if(!(add_goalkeeper(goal, field_length, field_width)) ||
		!(add_defenders(defenders, goal, field_length, field_width)) ||
		!(add_midfielders(midfielders, goal, field_length, field_width)))
		return false;
	return true;
}

void TeamFactory::add_view(std::map<const std::string, Animation> animMapper){
	log->info("TeamFactory: agregando vistas a los jugadores");
	for (player& p: team) {
		p.view = new PlayerView(animMapper, BasePlayer, p.model);
		p.controller = new PlayerControllerAI(p.model, p.view);
	}
}

bool TeamFactory::add_to_world(World& world){
	if (team.empty()){
		log->error("TeamFactory: intento de agregar al mundo un equipo vacio");
		return false;
	}
	
	for (player& p: team) {
		world.addEntity(p.model);
		world.addPlayerController(p.controller);
	}
	return true;
}

std::vector<player>& TeamFactory::get_team(){
	if (team.empty()){
		log->info("TeamFactory: se a requerido un equipo sin crear");
	}
	return team;
}

void TeamFactory::fill(std::vector<PlayerController*>& playerControllers)
{
	for (player& p : team) {
		playerControllers.push_back(p.controller);
	}
}

void TeamFactory::clear_team(){
	/*for (player& p: team) {
		delete p.model;
		delete p.view;
		delete p.controller;
	}*/
}

TeamFactory::~TeamFactory(){
	clear_team();
}
