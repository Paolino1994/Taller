#pragma once

#include <vector>

#include "CommandSender.h"
#include "common/GameConstants.h"
#include "Texture.h"
#include "Ball.h"
#include "Player.h"
#include "GameManager.h"

// Esto ahora va estar un poco mas relacionado con el juego
// Game / World
class World
{
private:
	// Las dimensiones del mapa en screen coordinates
	// en x: vamos de 0 -> width
	// en y: vamos de 0 -> height
	// misma logica que la de renderizacion (x aumenta a derecha, y aumenta para abajo)
	int width;
	int height;

    // El fondo (la cancha)
    Texture* background;
	// Tenemos una pelota ya desde el principio (controller por ahora que funca como un struct de model + view)
	Ball ball;
    // Objetos comunes
    std::vector<Entity*> entities; //queda todavia, pero pensar el uso de esto

	std::map<Player_ID, Player> players; // [static_cast<std::underlying_type<Team>::type>(Team::__LENGTH__)];

	// Guardo esto para poder crear Players despues
	player_data_t player_data;
	std::map<const std::string, Animation> playerAnimationMappers[static_cast<std::underlying_type<Team>::type>(Team::__LENGTH__)];

	std::vector<Texture*> playerIndicators;

	std::map<char, Texture*> miniMapIndicators;

public:
	//pendiente un refactor
    World(int width, int height, Texture* background, std::vector<Texture*>& playerIndicators, std::map<const std::string, Animation>& ballAnimMapper, Texture* ballMiniMap,
		std::map<const std::string, Animation>& teamAnimMapperHOME, std::map<const std::string, Animation>& teamAnimMapperAWAY,
		Texture* miniMapIndicatorHome, Texture* miniMapIndicatorAway,
		player_data_t player_data);
    ~World();

    void addEntity(Entity* entity);
    
    Texture* getBackground();
	Ball& getBall();
    std::vector<Entity*>& getEntities() ;
	std::map<Player_ID, Player>& getPlayers() ;

	void update(CommandSender& commandSender);

    
    int getWidth();
    int getHeight();
};

