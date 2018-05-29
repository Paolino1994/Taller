#include <algorithm>
#include <iostream>

#include "World.h"

World::World(int width, int height, Texture* background, std::vector<Texture*>& _playerIndicators, std::map<const std::string, Animation>& ballAnimMapper, Texture* ballMiniMap,
	std::map<const std::string, Animation>& teamAnimMapperHOME, std::map<const std::string, Animation>& teamAnimMapperAWAY, Texture* miniMapIndicatorHOME, Texture* miniMapIndicatorAWAY,
	player_data_t player_data):
	width(width),
	height(height),
    background(background),
	ball(Ball(ballAnimMapper, ballMiniMap)),
    entities(std::vector<Entity*>()),
	players(std::map<Player_ID, Player>()),
	player_data(player_data),
	playerIndicators(std::vector<Texture*>()),
	miniMapIndicators(std::map<char, Texture*>())
	{
	this->playerIndicators.reserve(4); // REFACTOR
	playerAnimationMappers[static_cast<std::underlying_type<Team>::type>(Team::HOME)] = teamAnimMapperHOME;
	playerAnimationMappers[static_cast<std::underlying_type<Team>::type>(Team::AWAY)] = teamAnimMapperAWAY;
	playerIndicators = _playerIndicators;
	miniMapIndicators['H'] = miniMapIndicatorHOME;
	miniMapIndicators['A'] = miniMapIndicatorAWAY;
}

World::~World()
{
}

void World::addEntity(Entity* entity){
    entities.push_back(entity);
}

Texture* World::getBackground() {
    return background;
}

Ball& World::getBall()
{
	return ball;
}

std::vector<Entity*>& World::getEntities() {
    return entities;
}

std::map<Player_ID, Player>& World::getPlayers() {
	return players;
}

void World::update(CommandSender& commandSender) {

	if (!commandSender.updateModel()) {
		return;
	}

	model_data_t modelData = commandSender.getModelData();

	std::vector<player_view_data_t>& playerViewData = modelData.playerViewData;
	ball_view_data_t& ball_view_data = modelData.ballViewData;

	for (player_view_data_t& player_view_data : playerViewData) {
		auto existing_player = players.find(player_view_data.playerId);
		if (existing_player != players.end()) { //Existe
			existing_player->second.update(player_view_data);
		}
		else {
			players.emplace(std::piecewise_construct,
				std::make_tuple(player_view_data.playerId),
				std::make_tuple(playerAnimationMappers[static_cast<std::underlying_type<Team>::type>(player_view_data.team)], this->player_data, playerIndicators, (player_view_data.team == Team::HOME) ? miniMapIndicators.at('H') : miniMapIndicators.at('A')));
		}
	}

	ball.update(ball_view_data);
	
	for (EventID eventId : modelData.events) {
		// hacer algo con el evento: 
		// ver si del lado del cliente tambien conviene meter patron tipo Observer para manejar eventos,
		// pero trataremos de no
		switch (eventId)
		{
		case EventID::KICK: {
			std::cout << "Server me avisa: Alguien pateo la bocha recien!" << std::endl;
			break;
		}
		default:
			break;
		}
	}

}

int World::getWidth(){
    return width;
}

int World::getHeight(){
    return height;
}

