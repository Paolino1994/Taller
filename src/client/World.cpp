#include <algorithm>

#include "World.h"

World::World(int width, int height, Texture* background, Texture* playerSelectedTexture, std::map<const std::string, Animation>& ballAnimMapper,
	std::map<const std::string, Animation>& teamAnimMapperHOME, std::map<const std::string, Animation>& teamAnimMapperAWAY,
	player_data_t player_data):
	width(width),
	height(height),
    background(background),
	playerSelectedTexture(playerSelectedTexture),
	ball(Ball(ballAnimMapper)),
    entities(std::vector<Entity*>()),
	players(std::map<Player_ID, Player>()),
	player_data(player_data)
{
	playerAnimationMappers[static_cast<std::underlying_type<Team>::type>(Team::HOME)] = teamAnimMapperHOME;
	playerAnimationMappers[static_cast<std::underlying_type<Team>::type>(Team::AWAY)] = teamAnimMapperAWAY;
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

Texture* World::getPlayerSelectedTexture() {
    return playerSelectedTexture;
}

void World::setPlayerSelectedTexture(Texture* texture) {
    playerSelectedTexture = texture;
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
				std::make_tuple(playerAnimationMappers[static_cast<std::underlying_type<Team>::type>(player_view_data.team)], this->player_data));
		}
	}

	ball.update(ball_view_data);
	
}

int World::getWidth(){
    return width;
}

int World::getHeight(){
    return height;
}

