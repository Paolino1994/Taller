#include <algorithm>
#include <iostream>

#include "World.h"
#include "../common/GameConstants.h"
#include "SoundManager.h"

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

void World::update(model_data_t& newModelData) {

	std::vector<player_view_data_t>& playerViewData = newModelData.playerViewData;
	ball_view_data_t& ball_view_data = newModelData.ballViewData;
	game_manager_data_t& game_manager_data = newModelData.gameManagerData;

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

	GameManager::get_instance()->update(game_manager_data);
}

void World::handleEvents(std::vector<EventID>& events)
{
	for (EventID eventId : events) {
		// TODO: hacer algo con el evento: en principio para sonidos y alguna visualizaci�n extra (tipo texto de Gol)
		// ver si del lado del cliente tambien conviene meter patron tipo Observer para manejar eventos,
		// pero trataremos de no
		switch (eventId)
		{
		case EventID::KICK: {
			SoundManager::get_instance()->playSound(SoundEffect::SE_BALL_KICK);
			std::cout << "Server me avisa: Alguien pateo la bocha recien!" << std::endl;
			break;
		}
		case EventID::GOAL: {
			GameManager::get_instance()->setDisplayGoalText(3);
			SoundManager::get_instance()->playSound(SoundEffect::SE_GOAL_CROWD);
			std::cout << "Server me avisa: Metieron gol!" << std::endl;
			break;
		}
		case EventID::PERIOD_END: {
			SoundManager::get_instance()->playSound(SoundEffect::SE_WHISTLE);
			std::cout << "Server me avisa: Termino un tiempo!" << std::endl;
			break;
		}
		case EventID::PERIOD_START: {
			SoundManager::get_instance()->playSound(SoundEffect::SE_WHISTLE);
			std::cout << "Server me avisa: Empieza un tiempo!" << std::endl;
			break;
		}
		case EventID::GOAL_KICK: {
			GameManager::get_instance()->setDisplayGoalKickText(2);
			SoundManager::get_instance()->playSound(SoundEffect::SE_WHISTLE);
			std::cout << "Server me avisa: Salio por el costado, saca el arquero!" << std::endl;
			break;
		}
		case EventID::POSTHIT: {
			SoundManager::get_instance()->playSound(SoundEffect::SE_POSTHIT);
			std::cout << "Server me avisa: Que hay palo" << std::endl;
			break;
		}
		case EventID::GAME_END: {
			GameManager::get_instance()->setShowGoalStats(true);
			std::cout << "Server me avisa: Termino el juego, momento de mostrar estadisticas" << std::endl;
			break;
		}
		case EventID::GAME_RESTART: {
			GameManager::get_instance()->setShowGoalStats(false);
			std::cout << "Server me avisa: Va a reiniciar el juego, dejo de mostrar estadisticas" << std::endl;
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

