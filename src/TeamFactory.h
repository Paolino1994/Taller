#ifndef __FORMATION_CREATOR_H__
#define __FORMATION_CREATOR_H__

#include <map>
#include <string>
#include "Animation.h"

#include "PlayerModel.h"
#include "PlayerView.h"
#include "PlayerController.h"
#include "World.h"

#define LEFT_GOAL 0
#define RIGHT_GOAL 1

struct player {
	PlayerModel* model;
	PlayerView* view;
	PlayerController* controller;
};

class TeamFactory {
private:
	std::vector<player> team;
	bool add_goalkeeper(int goal, int field_length, int field_width);
	bool add_defenders(int quantity, int goal, int field_length, int field_width);
	bool add_midfielders(int quantity, int goal, int field_length, int field_width);
	bool add_forwards(int quantity, int goal, int field_length, int field_width);

public:
	bool create(int defenders, int midfielders, int forwards, int goal, int field_length, int field_width);
	bool create(int defenders, int midfielders, int goal, int field_length, int field_width);
	void add_view(std::map<const std::string, Animation> animMapper);
	bool add_to_world(World& world);
	std::vector<player> get_team();
};

#endif
