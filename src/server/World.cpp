#include "World.h"
#include "BallController.h"
#include "TeamFactory.h"
#include "PlayerControllerHuman.h"
#include "PlayerControllerAI.h"
#include "../common/Log.h"
#include <algorithm>
#include <iostream>
#include "scripted/PlayerKickOffSetupState.h"

std::vector<std::string> separarFormacion(const std::string& str, const char& ch) ;

World::World(int width, int height, std::map<const std::string, Animation> ballAnimMapper):
	systems(std::vector<std::shared_ptr<System>>()),
	ball(BallController(width/2, height/2, ballAnimMapper)),
    entities(std::vector<Entity*>()),
	pControllers(std::vector<PlayerController*>()),
    width(width),
    height(height)
{
	this->registerTo(EventID::PERIOD_END);
}

World::~World()
{
	for (auto controllers : playerControllers) {
		for (PlayerController* controller : controllers) {
			delete controller->getModel();
			delete controller->getView();
			delete controller;
		}
	}
	this->unregisterFrom(EventID::PERIOD_END);
}

void World::createTeam(Team team, int defenders, int midfielders, int forwards, player_data_t playerData, std::map<const std::string, Animation>& animMapper)
{
	size_t teamIndex = static_cast<std::underlying_type<Team>::type>(team);
	if (team != Team::__LENGTH__ && playerControllers[teamIndex].size() == 0) {
		TeamFactory teamFactory(playerData);
		teamFactory.create(defenders, midfielders, forwards, team, this->getWidth(), this->getHeight());
		teamFactory.add_view(animMapper);
		teamFactory.fill(playerControllers[teamIndex]);

		for (auto controller : playerControllers[teamIndex]) {
			pControllers.push_back(controller);
			controller->setScriptedState(new PlayerKickOffSetupState(*controller->getModel(), Team::HOME,
				GameManager::getKickOffSideAfterPeriodEnd()));
		}
	}
	else {
		Log::get_instance()->info("Intentaron recrear un equipo, no lo permito y me quedo con el anterior");
	}
}

bool World::noOneHasControlOfTheBall(std::vector<PlayerController*>& teamControllers){
    for(PlayerController* player : teamControllers){
        if(player->hasControlOfTheBall()){
            return false;
        }
    }
    return true;
}

PlayerController* World::injectHumanController(Team team, User_ID userId)
{
    std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(team)];

    for (size_t i = teamControllers.size(); i-- > 0 ;)
    {
        if (teamControllers[i]->isControllable()) {
            PlayerController* other = teamControllers[i];
            PlayerControllerHuman* human = new PlayerControllerHuman(other, *this, userId);
            teamControllers[i] = human;
            delete other;
            return human;
        }
    }

    return nullptr;
}

bool World::playerIsOnRange(PlayerController* cont,PlayerController* controllerToSwap){
    //PlayerModel* model=cont->getModel();
    int x=ball.getModel().getX();
    int y=ball.getModel().getY();
    int xCon=cont->getModel()->getCenterX();
    int yCon=cont->getModel()->getCenterY();
    ////std::cout<<"BALL X:"<<x<<" Y:"<<y<<std::endl;
    ////std::cout<<"X:"<<xCon<<" Y:"<<yCon<<std::endl;
    //int xRange=getRangeToChange(x,200);
    //int yRange=getRangeToChange(y,150);
    if(abs(x-xCon)<YAML::SCREEN_WIDTH/2 && abs(y-yCon)<YAML::SCREEN_HEIGHT/2){
        ////std::cout<<"vale Controllador "<<std::endl;
        return true;
    }
    return false;

}

PlayerController* World::getPlayerToPass(PlayerController * controllerToSwap){
    Team team = controllerToSwap->getModel()->getTeam();
    std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(team)];

	ptrdiff_t index = std::distance(teamControllers.begin(), std::find(teamControllers.begin(), teamControllers.end(), controllerToSwap));
    double anguloActual=50000000;
    int indexAUsar = -1;
    if (index >= (long long)teamControllers.size()) {
		Log::get_instance()->info("El controller que se quiso swapear no esta en mi listado de controllers del equipo correspondiente");

	}

	size_t controllerToSwapIndex = index;
	//bool comparacion;
	index=-1;
	do
	{ // en el peor caso volvemos a el que estabamos controlando recien
		//++index;
		index++;
		if (index == (long long)teamControllers.size()) {
			index = controllerToSwapIndex;
			break;
		}
		if(playerIsOnPassRange(teamControllers[index],controllerToSwap)&& teamControllers[index] != controllerToSwap && playerIsOnRange(teamControllers[index],controllerToSwap) /*&& teamControllers[index]->isControllable()*/){
            ////std::cout<<"Player "<<index<<" DistActual "<<distanciaActual<<" Distancia "<<getDistance(teamControllers[index]->getModel(),getBall().getModel())<<std::endl;
            if(abs(getAngle(teamControllers[index]->getModel(),getBall().getModel())-(360-ball.getModel().getAngle()))<anguloActual){
                indexAUsar=index;
                anguloActual=getAngle(teamControllers[index]->getModel(),getBall().getModel());
                ////std::cout<<"Angulo:"<<anguloActual<<std::endl;
            }
		}
		/*comparacion=!playerIsOnRange(teamControllers[index],controllerToSwap);
		comparacion=(comparacion==(teamControllers[index] != controllerToSwap));
		comparacion=(comparacion==!teamControllers[index]->isControllable());*/
		//comparacion=comparacion==false;

	} //while (!playerIsOnRange(teamControllers[index],controllerToSwap) && teamControllers[index] != controllerToSwap && !teamControllers[index]->isControllable()); // sin chequeos de camara por ahora -> igual se sacaba para la fase 2
	while(true);
    if(indexAUsar!=-1){
        return teamControllers[indexAUsar];

    }else{
        return teamControllers[controllerToSwapIndex];
    }


}

PlayerController* World::getPlayerToPassLong(PlayerController * controllerToSwap){
    Team team = controllerToSwap->getModel()->getTeam();
    std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(team)];

    ptrdiff_t index = std::distance(teamControllers.begin(), std::find(teamControllers.begin(), teamControllers.end(), controllerToSwap));
    double anguloActual=50000000;
    int indexAUsar = -1;
    if (index >= (long long)teamControllers.size()) {
        Log::get_instance()->info("El controller que se quiso swapear no esta en mi listado de controllers del equipo correspondiente");

    }

    size_t controllerToSwapIndex = index;
    //bool comparacion;
    index=-1;
    do
    { // en el peor caso volvemos a el que estabamos controlando recien
        //++index;
        index++;
        if (index == (long long)teamControllers.size()) {
            index = controllerToSwapIndex;
            break;
        }
        if(playerIsOnPassRange(teamControllers[index],controllerToSwap)&& teamControllers[index] != controllerToSwap && !playerIsOnRange(teamControllers[index],controllerToSwap) /*&& teamControllers[index]->isControllable()*/){
            ////std::cout<<"Player "<<index<<" DistActual "<<distanciaActual<<" Distancia "<<getDistance(teamControllers[index]->getModel(),getBall().getModel())<<std::endl;
            if(abs(getAngle(teamControllers[index]->getModel(),getBall().getModel())-(360-ball.getModel().getAngle()))<anguloActual){
                indexAUsar=index;
                anguloActual=getAngle(teamControllers[index]->getModel(),getBall().getModel());
                ////std::cout<<"Angulo:"<<anguloActual<<std::endl;
            }
        }
        /*comparacion=!playerIsOnRange(teamControllers[index],controllerToSwap);
        comparacion=(comparacion==(teamControllers[index] != controllerToSwap));
        comparacion=(comparacion==!teamControllers[index]->isControllable());*/
        //comparacion=comparacion==false;

    } //while (!playerIsOnRange(teamControllers[index],controllerToSwap) && teamControllers[index] != controllerToSwap && !teamControllers[index]->isControllable()); // sin chequeos de camara por ahora -> igual se sacaba para la fase 2
    while(true);
    if(indexAUsar!=-1){
        return teamControllers[indexAUsar];
        ////std::cout<<indexAUsar<<std::endl;
    }else{
        return teamControllers[controllerToSwapIndex];
    }

}

bool World::ejectController(PlayerController * playerController, User_ID userId)
{
	std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(playerController->getModel()->getTeam())];

	for (size_t i = 0; i < teamControllers.size(); i++)
	{
		if (teamControllers[i] == playerController) {
			PlayerControllerAI* aiController = new PlayerControllerAI(playerController);
			teamControllers[i] = aiController;
			aiController->getModel()->setHasControlOfTheBall(false);
			delete playerController;
			return true;
		}
	}

	return false;
}

void World::swap(PlayerController * controllerToSwap)
{
	Team team = controllerToSwap->getModel()->getTeam();
	std::vector<PlayerController*>& teamControllers = playerControllers[static_cast<std::underlying_type<Team>::type>(team)];

	ptrdiff_t index = std::distance(teamControllers.begin(), std::find(teamControllers.begin(), teamControllers.end(), controllerToSwap));

	if (index >= (long long)teamControllers.size()) {
		Log::get_instance()->info("El controller que se quiso swapear no esta en mi listado de controllers del equipo correspondiente");
		return;
	}

	size_t controllerToSwapIndex = index;
	////std::cout<<"-----------------------------------------------------"<<std::endl;
	index=-1;
	do
	{ // en el peor caso volvemos a el que estabamos controlando recien
		//++index;
		index++;
		if (index == (long long)teamControllers.size()) {
			index = controllerToSwapIndex;
			break;
		}
		if(teamControllers[index] != controllerToSwap && playerIsOnRange(teamControllers[index],controllerToSwap) && teamControllers[index]->isControllable()){
			break;
		}
		/*comparacion=!playerIsOnRange(teamControllers[index],controllerToSwap);
		comparacion=(comparacion==(teamControllers[index] != controllerToSwap));
		comparacion=(comparacion==!teamControllers[index]->isControllable());*/
		//comparacion=comparacion==false;

	} //while (!playerIsOnRange(teamControllers[index],controllerToSwap) && teamControllers[index] != controllerToSwap && !teamControllers[index]->isControllable()); // sin chequeos de camara por ahora -> igual se sacaba para la fase 2
	while(true);
	if (teamControllers[index] != controllerToSwap) {
		controllerToSwap->swap(teamControllers[index]);
		// camera.follow(controlled->getEntity()); lo hacemos en el main
		auto temp = teamControllers[index];
		teamControllers[index] = controllerToSwap;
		teamControllers[controllerToSwapIndex] = temp;
	}
}



void World::addEntity(Entity* entity){
    entities.push_back(entity);
}



void World::addPlayerController(PlayerController* pController){
	pControllers.push_back(pController);
}


BallController& World::getBall()
{
	return ball;
}

std::vector<Entity*>& World::getEntities() {
    return entities;
}

std::vector<PlayerController*>& World::getPlayerControllers() {
	// TODO
	// TEMP, pendiente una modificacion, pero con la arq cliente-servidor esto probablemente ni exista
	pControllers = std::vector<PlayerController*>();

	for (std::vector<PlayerController*>& controllers : playerControllers) {
		pControllers.insert(pControllers.end(), controllers.begin(), controllers.end());
	}
	
	return pControllers;
}

void World::update(double dt)
{
	// Player update
	for (std::vector<PlayerController*>& controllers: playerControllers) {
		for (auto player: controllers) {
			player->update(dt, this->getWidth(), this->getHeight(), ball.getModel().getX(),ball.getModel().getY());
		}
	}

	// Ball update
	ball.update(dt, this->getWidth(), this->getHeight());
	//updateBallController();
	
	// Systems update
	for (auto system : systems) {
		system->process(dt);
	}
}

void World::serialize(model_data_t & modelData)
{
	for (auto controllers : playerControllers) {
		for (auto player : controllers) {
			player_view_data_t playerData;
			player->serialize(playerData);
			modelData.playerViewData.push_back(playerData);
		}
	}

	ball.serialize(modelData.ballViewData);

    GameManager::get_instance().serialize(modelData.gameManagerData);
}

int World::getWidth(){
    return width;
}

int World::getHeight(){
    return height;
}

void World::addSystem(std::shared_ptr<System> system)
{
	this->systems.push_back(system);
}

bool World::playerIsOnPassRange(PlayerController *&controller, PlayerController *controllerToSwap) {
    int x=ball.getModel().getX();
    int y=ball.getModel().getY();
    int xCon=controller->getModel()->getCenterX();
    int yCon=controller->getModel()->getCenterY();
    double ballAngle=ball.getModel().getAngle();
    if(ballAngle<0){
        ballAngle=360+ballAngle;
    }
    double angulo=atan2(yCon-y,xCon-x);
    double angleToPlayer=(angulo*180/M_PI);
    //double angleToPlayer = getAngle(controller->getModel(), ball.getModel());
    if(angleToPlayer<0){
        angleToPlayer=360+angleToPlayer;
    }
    ////std::cout<<"Angle: "<<ballAngle<<" AngleToPlayer "<<angleToPlayer<<std::endl;
    if (abs(angleToPlayer-ballAngle)<40){
        ////std::cout<<"Angulo Correcto"<<std::endl;
        return true;
    }

    ////std::cout<<"Angle: "<<ballAngle<<" X "<<x<<" xCon "<<xCon<<" Y "<<y<<" yCon "<<yCon<<std::endl;

    return false;
}

double World::getAngle(PlayerModel *pModel, BallModel &model) {
    int x1 = model.getX();
    int x2 = pModel->getCenterX();
    int y1=model.getY();
    int y2=pModel->getCenterY();
    double angulo=atan2(y2-y1,x2-x1);
    angulo=360-((angulo*180/M_PI) + 90);
    return angulo;

}


void World::changeFormation(Team team, FIELD_POSITION goalSide, std::string formation){

    std::vector<std::string> separados;
    separados = separarFormacion(formation, '-');
    int mitadDeCancha_x = YAML::FIELD_CENTER_X;
    int longTotalCancha_y = YAML::FIELD_HEIGHT;
    // int defensores = stoi(separados[0]); Para esta primera version de la reformacion no hace falta porque lo unico que cambia es el mediocampo
    int mediocampistas = stoi(separados[1]);
    // int delanteros = stoi(separados[2]);

    int direccion_x = 0;

    // Arquero
    direccion_x = (goalSide == FIELD_POSITION::LEFT) ? -1 : 1; // Aca seteo de que lado de la cancha lo pongo  
    playerControllers[(int)team][0]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 750));
    playerControllers[(int)team][0]->getModel()->setInitial_y(longTotalCancha_y / 2);
    playerControllers[(int)team][0]->getModel()->setRole('G');


    // Defensores (siempre deberia ser 3, por ahora)
    direccion_x = goalSide == 0 ? -1 : 1;  
    playerControllers[(int)team][1]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 500));
    playerControllers[(int)team][1]->getModel()->setInitial_y(longTotalCancha_y / 4);
    playerControllers[(int)team][1]->getModel()->setRole('D');
    playerControllers[(int)team][2]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 500));
    playerControllers[(int)team][2]->getModel()->setInitial_y(longTotalCancha_y / 2);
    playerControllers[(int)team][2]->getModel()->setRole('D');
    playerControllers[(int)team][3]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 500));
    playerControllers[(int)team][3]->getModel()->setInitial_y((longTotalCancha_y / 4) * 3);
    playerControllers[(int)team][3]->getModel()->setRole('D');


    // Mediocampistas 
    direccion_x = goalSide == 0 ? 1 : -1; // cambio la direccion para que esten parados del lado que atacan (ofensivos)
    if (mediocampistas == 1) { // Si es un mediocampista entonces son 2 delanteros
        playerControllers[(int)team][4]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 100));
        playerControllers[(int)team][4]->getModel()->setInitial_y(longTotalCancha_y / 2);
        playerControllers[(int)team][4]->getModel()->setRole('M');

        
        //Delanteros
        playerControllers[(int)team][5]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 400));
        playerControllers[(int)team][5]->getModel()->setInitial_y(longTotalCancha_y / 3);
        playerControllers[(int)team][5]->getModel()->setRole('F');
        playerControllers[(int)team][6]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 400));
        playerControllers[(int)team][6]->getModel()->setInitial_y((longTotalCancha_y / 3) * 2);
        playerControllers[(int)team][6]->getModel()->setRole('F');


    } else if (mediocampistas == 2) { //Dos mediocampistas entonces 1 delantero

        playerControllers[(int)team][4]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 100));
        playerControllers[(int)team][4]->getModel()->setInitial_y(longTotalCancha_y / 3);
        playerControllers[(int)team][4]->getModel()->setRole('M');
        playerControllers[(int)team][5]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 100));
        playerControllers[(int)team][5]->getModel()->setInitial_y((longTotalCancha_y / 3) * 2);
        playerControllers[(int)team][5]->getModel()->setRole('M');


        //Delanteros
        playerControllers[(int)team][6]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 400));
        playerControllers[(int)team][6]->getModel()->setInitial_y(longTotalCancha_y / 2);
        playerControllers[(int)team][6]->getModel()->setRole('F');


    } else if (mediocampistas == 3) { // Tres mediocampistas entonces 0 delanteros

        playerControllers[(int)team][4]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 100));
        playerControllers[(int)team][4]->getModel()->setInitial_y(longTotalCancha_y / 4);
        playerControllers[(int)team][4]->getModel()->setRole('M');
        playerControllers[(int)team][5]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 100));
        playerControllers[(int)team][5]->getModel()->setInitial_y(longTotalCancha_y / 2);
        playerControllers[(int)team][5]->getModel()->setRole('M');
        playerControllers[(int)team][6]->getModel()->setInitial_x(mitadDeCancha_x + (direccion_x * 100));
        playerControllers[(int)team][6]->getModel()->setInitial_y((longTotalCancha_y / 4) * 3);
        playerControllers[(int)team][6]->getModel()->setRole('M');
    }
}


void World::switchTeamSides()
{
	const int centerX = YAML::FIELD_CENTER_X;
	const int centerY = YAML::FIELD_CENTER_Y;

	for (std::vector<PlayerController*>& controllers : playerControllers) {
		for (auto player : controllers) {
			PlayerModel* playerModel = player->getModel();
			playerModel->setInitial_x(centerX + (centerX - playerModel->getInitial_x()));
			playerModel->setInitial_y(centerY + (centerY - playerModel->getInitial_y()));
		}
	}
}

void World::setSetPiecePosition(Team team, FIELD_POSITION goalSide, SET_PIECE setPiece){

    int mitadDeCancha_x = YAML::FIELD_CENTER_X;

    int direccion_x = 0; 

    direccion_x = (goalSide == FIELD_POSITION::LEFT) ? -1 : 1; // Aca seteo de que lado de la cancha lo pongo  

    PlayerModel *playerModel = nullptr; 

    if(setPiece == SET_PIECE::KICKOFF) {
        for (auto playerController : playerControllers[(int)team]) {
            playerModel = playerController->getModel();
            playerModel->setKickOff_y(playerModel->getInitial_y());
            switch (playerModel->getRole()) {
                case 'G':
                    playerModel->setKickOff_x(mitadDeCancha_x + (direccion_x * 750));
                break;
                case 'D':
                    playerModel->setKickOff_x(mitadDeCancha_x + (direccion_x * 500));
                break;
                case 'M':
                    playerModel->setKickOff_x(mitadDeCancha_x + (direccion_x * 300));
                break;
                case 'F':
                    playerModel->setKickOff_x(mitadDeCancha_x + (direccion_x * 100));
                break;
                default:
                break;
            }
        }
    } else if(setPiece == SET_PIECE::GOALKICK) {
        for (auto playerController : playerControllers[(int)team]) {
            playerModel = playerController->getModel();
            playerModel->setKickOff_y(playerModel->getInitial_y());
            switch (playerModel->getRole()) {
                case 'G':
                    playerModel->setKickOff_x(mitadDeCancha_x + (direccion_x * 750));
                break;
                case 'D':
                    playerModel->setKickOff_x(mitadDeCancha_x + (direccion_x * 500));
                break;
                case 'M':
                    playerModel->setKickOff_x(mitadDeCancha_x + (direccion_x * -100));
                break;
                case 'F':
                    playerModel->setKickOff_x(mitadDeCancha_x + (direccion_x * -300));
                break;
                default:
                break;
            }
        }
    }

}

void World::setZonesDistances(Team team){
    YAMLReader& yamlReader = YAMLReader::get_instance();
    PlayerModel *playerModel = nullptr; 
    for (auto playerController : playerControllers[(int)team]) {
            playerModel = playerController->getModel();
            playerModel->setKickOff_y(playerModel->getInitial_y());
            switch (playerModel->getRole()) {
                case 'G':
                    playerModel->setDefence_distance_x(yamlReader.getDefensaArqueroX());
                    playerModel->setDefence_distance_y(yamlReader.getDefensaArqueroY());
                    playerModel->setAtack_distance_x(yamlReader.getAtaqueArqueroX());
                    playerModel->setAtack_distance_y(yamlReader.getAtaqueArqueroY());
                break;
                case 'D':
                    playerModel->setDefence_distance_x(yamlReader.getDefensaDefensorX());
                    playerModel->setDefence_distance_y(yamlReader.getDefensaDefensorX());
                    playerModel->setAtack_distance_x(yamlReader.getAtaqueDefensorX());
                    playerModel->setAtack_distance_y(yamlReader.getAtaqueDefensorY());
                break;
                case 'M':
                    playerModel->setDefence_distance_x(yamlReader.getDefensaMediocampistaX());
                    playerModel->setDefence_distance_y(yamlReader.getDefensaMediocampistaX());
                    playerModel->setAtack_distance_x(yamlReader.getAtaqueMediocampistaX());
                    playerModel->setAtack_distance_y(yamlReader.getAtaqueMediocampistaY());
                break;
                case 'F':
                    playerModel->setDefence_distance_x(yamlReader.getDefensaDelanteroX());
                    playerModel->setDefence_distance_y(yamlReader.getDefensaDelanteroX());
                    playerModel->setAtack_distance_x(yamlReader.getAtaqueDelanteroX());
                    playerModel->setAtack_distance_y(yamlReader.getAtaqueDelanteroY());
                break;
                default:
                break;
            }
        }
}

void World::handle(PeriodEndEvent & e)
{
	// Hacemos que cambien de lado!
	GameManager::get_instance().switchTeamFieldPositions();
	this->switchTeamSides();
	this->setSetPiecePosition(SET_PIECE::KICKOFF);
}

std::vector<std::string> separarFormacion(const std::string& str, const char& ch) {
    std::string siguiente;
    std::vector<std::string> resultado;

    for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
        if (*it == ch) {
            if (!siguiente.empty()) {
                resultado.push_back(siguiente);
                siguiente.clear();
            }
        } else {
            siguiente += *it;
        }
    }
    if (!siguiente.empty())
        resultado.push_back(siguiente);
    return resultado;
}
