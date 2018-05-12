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

const std::map<Player_ID, Player>& World::getPlayers() {
	return players;
}

void World::_update(Protocol& protocol, bool goAgain) {

	protocol.read();

	Request request = protocol.request();

	if (request == Request::PLAYER_VIEW_UPDATE) {
		const player_view_data_t* player_view_data = reinterpret_cast<const player_view_data_t*>(protocol.dataBuffer());
		size_t player_view_data_len = protocol.dataLength() / sizeof(player_view_data);
		for (size_t i = 0; i < player_view_data_len; ++i) {
			// Faltaria chequear si me sacan algun jugador, pero no debeia pasar!

			auto existing_player = players.find(player_view_data[i].playerId);
			if (existing_player != players.end()) { //Existe
				existing_player->second.update(player_view_data[i]);
			}
			else {
				players.emplace(std::piecewise_construct,
					std::make_tuple(player_view_data[i].playerId),
					std::make_tuple(playerAnimationMappers[static_cast<std::underlying_type<Team>::type>(player_view_data[i].team)], this->player_data));
			}
		}
	}
	else if (request == Request::BALL_VIEW_UPDATE) {
		const ball_view_data_t ball_view_data = *reinterpret_cast<const ball_view_data_t*>(protocol.dataBuffer());
		ball.update(ball_view_data);
	}

	if (goAgain) { // En una se actualizan los jugadores, en la otra la pelota. El orden no importa
		this->_update(protocol, false); 
	}
}

void World::update(Protocol& protocol)
{
	this->_update(protocol, true);
}

int World::getWidth(){
    return width;
}

int World::getHeight(){
    return height;
}

