#include "TeamFactory.h"

bool TeamFactory::add_goalkeeper(int goal, int field_length, int field_width){
	PlayerModel* model;

	switch (goal) {
		case LEFT_GOAL:
			model = new PlayerModel(DEFAULT_PLAYER, field_length * 1.5 / 22, field_width * 21/ 44);
			break;
		case RIGHT_GOAL:
			model = new PlayerModel(DEFAULT_PLAYER, field_length * 20.5 / 22, field_width * 21 / 44);
			break;
		default:
			return false;
	}

	player goalkeeper;
	goalkeeper.model = model;
	team.push_back(goalkeeper);
	return true;
}


bool TeamFactory::add_defenders(int quantity, int goal, int field_length, int field_width){
	if (quantity != 3) {
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
			return false;
	}

	PlayerModel* model;
	player defender;

	for (int i = 0; i < quantity; i++){
		position_y = ((field_width * 5 / 8) * i / 2) + 	field_width / 7;
		model = new PlayerModel(DEFAULT_PLAYER, position_x, position_y);
		defender.model = model;
		team.push_back(defender);
	}
	return true;
}

bool TeamFactory::add_midfielders(int quantity, int goal, int field_length, int field_width){
	if (quantity <= 0 || quantity > 3) {
		return false;
	}

	int position_x, position_y;

	switch (goal) {
		case LEFT_GOAL:
			position_x = field_length * 10 / 22;
			break;
		case RIGHT_GOAL:
			position_x = field_length * 12 / 22;
			break;
		default:
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
		model = new PlayerModel(DEFAULT_PLAYER, position_x, position_y);
		midfielder.model = model;
		team.push_back(midfielder);
	}

	return true;
}

bool TeamFactory::add_forwards(int quantity, int goal, int field_length, int field_width){
	if (quantity <= 0 || quantity > 2) {
		return false;
	}

	int position_x, position_y;

	switch (goal) {
		case LEFT_GOAL:
			position_x = field_length * 15 / 22;
			break;
		case RIGHT_GOAL:
			position_x = field_length * 7 / 22;
			break;
		default:
			return false;
	}

	PlayerModel* model;
	player forward;

	for (int i = 0; i < quantity; i++){
		switch (quantity) {
			case 1:
				position_y = field_width / 2;
				break;
			case 2:
				position_y = ((field_width * 5 / 8) * (1 + 4 * i) / 5) + field_width / 8;
				break;
			default:
				return false;
		}
		model = new PlayerModel(DEFAULT_PLAYER, position_x, position_y);
		forward.model = model;
		team.push_back(forward);
	}

	return true;
}

bool TeamFactory::create(int defenders, int midfielders, int forwards, int goal, int field_length, int field_width){
	team.clear();
	if(!(add_goalkeeper(goal, field_length, field_width)) ||
		!(add_defenders(defenders, goal, field_length, field_width)) ||
		!(add_midfielders(midfielders, goal, field_length, field_width)) ||
		!(add_forwards(forwards, goal, field_length, field_width)))
		return false;
	return true;
}

bool TeamFactory::create(int defenders, int midfielders, int goal, int field_length, int field_width){
	team.clear();
	if(!(add_goalkeeper(goal, field_length, field_width)) ||
		!(add_defenders(defenders, goal, field_length, field_width)) ||
		!(add_midfielders(midfielders, goal, field_length, field_width)))
		return false;
	return true;
}

void TeamFactory::add_view(std::map<const std::string, Animation> animMapper){
	for (player& p: team) {
		p.view = new PlayerView(animMapper, DEFAULT_PLAYER, p.model);
		p.controller = new PlayerController(p.model, p.view);
	}
}

bool TeamFactory::add_to_world(World& world){
	if (team.empty()){
		return false;
	}
	
	for (player& p: team) {
		world.addEntity(p.model);
		world.addPlayerController(p.controller);
	}
	return true;
}

std::vector<player>& TeamFactory::get_team(){
	return team;
}
