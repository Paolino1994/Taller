#pragma once

#include <vector>

#include "Texture.h"
#include "Entity.h"
#include "GameConstants.h"
#include "PlayerController.h"

// Esto ahora va estar un poco mas relacionado con el juego
// Game / World
class World
{
private:
    // El fondo (la cancha)
    Texture* background;
    // Jugador selecionado
    Texture* playerSelectedTexture;
    // Objetos comunes
    std::vector<Entity*> entities; //queda todavia, pero pensar el uso de esto

	std::vector<PlayerController*> playerControllers[Team::__LENGTH__];

    std::vector<PlayerController*> pControllers;
    // Las dimensiones del mapa en screen coordinates
	// en x: vamos de 0 -> width
	// en y: vamos de 0 -> height
	// misma logica que la de renderizacion (x aumenta a derecha, y aumenta para abajo)
    int width;
    int height;

public:
    World(int width, int height, Texture* background);
    ~World();

	// Creaci�n de elementos
	void createTeam(Team team, int defenders, int midfielders, int forwards, player_data_t playerData, std::map<const std::string, Animation>& animMapper);

	// Inyeccion de controller
	PlayerController* injectHumanController(Team team);

	// Swap controller
	void swap(PlayerController* playerController);

    void addEntity(Entity* entity);

    void addPlayerController(PlayerController* pController);
    void setPlayerSelectedTexture(Texture* texture) ;

    
    Texture* getPlayerSelectedTexture() ;
    Texture* getBackground() ;
    std::vector<Entity*>& getEntities() ;
    std::vector<PlayerController*>& getPlayerControllers() ;

	void update(double dt);

    
    int getWidth();
    int getHeight();

};

