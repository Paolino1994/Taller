#pragma once

#include <vector>

#include "Entity.h"
#include "BallController.h"
#include "common/GameConstants.h"
#include "PlayerController.h"
#include "System.h"
#include "GameManager.h"


// Esto ahora va estar un poco mas relacionado con el juego
// Game / World
// DEL LADO DEL SERVER:
// SIN Texturas colgando
class World
{
private:
	std::vector<std::shared_ptr<System>> systems;
	// Tenemos una pelota ya desde el principio (controller por ahora que funca como un struct de model + view)
	BallController ball;
	// Objetos comunes
	std::vector<Entity*> entities; //queda todavia, pero pensar el uso de esto

	std::vector<PlayerController*> playerControllers[static_cast<std::underlying_type<Team>::type>(Team::__LENGTH__)];

	std::vector<PlayerController*> pControllers;

	
	// Las dimensiones del mapa en screen coordinates
	// en x: vamos de 0 -> width
	// en y: vamos de 0 -> height
	// misma logica que la de renderizacion (x aumenta a derecha, y aumenta para abajo)
	int width;
	int height;
	bool playerIsOnRange(PlayerController* cont, PlayerController* controllerToSwap);
    bool noOneHasControlOfTheBall(std::vector<PlayerController*>& teamControllers);
    int controlCounter=0;

public:
	World(int width, int height, std::map<const std::string, Animation> ballAnimMapper);
	~World();

	// Creaci�n de elementos
	void createTeam(Team team, int defenders, int midfielders, int forwards, player_data_t playerData, std::map<const std::string, Animation>& animMapper);

	// Inyeccion de controller
	PlayerController* injectHumanController(Team team, User_ID userId);

	// Reseteamos un controller al default (actualmente controller AI)
	// if return  == true : invalido el playerController, le hicimos delete
	bool ejectController(PlayerController* playerController, User_ID userId);

	// Swap controller
	void swap(PlayerController* playerController);

	void addEntity(Entity* entity);

	void addPlayerController(PlayerController* pController);

	BallController& getBall();
	std::vector<Entity*>& getEntities();
	std::vector<PlayerController*>& getPlayerControllers();

	void update(double dt);
	void serialize(model_data_t& modelData);
    
    int getWidth();
    int getHeight();

	void addSystem(std::shared_ptr<System> system);
	//bool removeSystem(System* system); TODO: ver si es necesario!, o como mucho un disableSystem

    PlayerController* getPlayerToPassLong(PlayerController* controllerToSwap);
	PlayerController* getPlayerToPass(PlayerController* controllerToSwap);

	bool playerIsOnPassRange(PlayerController *&pController, PlayerController *pPlayerController);

    double getAngle(PlayerModel *pModel, BallModel &model);

	void changeFormation(Team team, FIELD_POSITION goalSide, std::string formation);

	void setSetPiecePosition(Team team, FIELD_POSITION goalSide, int setPiece);



};

