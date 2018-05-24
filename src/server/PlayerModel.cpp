//
// Created by federico on 11/04/18.
//

#include "PlayerModel.h"
#include "BallController.h"
#include "../common/GameConstants.h"
#include <cmath>
#include <iostream>
using namespace std;

Player_ID PlayerModel::nextPlayerId[static_cast<std::underlying_type<Team>::type>(Team::__LENGTH__)] = { 1,100 };

Player_ID PlayerModel::getNextPlayerId(Team team) {
	const size_t teamIndex = static_cast<std::underlying_type<Team>::type>(team);
	Player_ID pID = PlayerModel::nextPlayerId[teamIndex];
	PlayerModel::nextPlayerId[teamIndex] += 1;
	return pID;
}

PlayerModel::PlayerModel(Team team, const player_data_t player_data, double initial_x, double initial_y, int kickOff_x, int kickOff_y) :
	Entity(kickOff_x, kickOff_y),
	initial_x(initial_x),
	initial_y(initial_y),
	kickOff_x(kickOff_x),
	kickOff_y(kickOff_y),
	team(team),
	playerId(PlayerModel::getNextPlayerId(team)),
	widths(player_data.widths, std::end(player_data.widths)),
	heights(player_data.heights, std::end(player_data.heights)),
	velX(0),
	velY(0),
	state(STILL),
	angle(0.0),
	MAX_VEL_X(player_data.X_VELOCITY),
	MAX_VEL_Y(player_data.Y_VELOCITY),
	sweepDuration(player_data.SWEEP_DURATION),
	sweepTime(0.0),
	sweepVelX(0.0),
	sweepVelY(0.0),
	kickDuration(player_data.KICK_DURATION),
	kickTime(0.0),
	kickVelX(0.0),
	kickVelY(0.0),
	sprintVelocityMultiplier(player_data.SPRINT_VELOCITY_MULTIPLIER),
	velocityMultiplier(1.0)
{
	log = Log::get_instance();
}

PlayerModel::PlayerModel(Team team, const player_data_t player_data, double initial_x, double initial_y):
	Entity(initial_x, initial_y),
	initial_x(initial_x),
	initial_y(initial_y),
	kickOff_x(initial_x),
	kickOff_y(initial_y),
	team(team),
	playerId(PlayerModel::getNextPlayerId(team)),
	widths(player_data.widths, std::end(player_data.widths)),
	heights(player_data.heights, std::end(player_data.heights)),
	velX(0),
	velY(0),
	state(STILL),
	angle(0.0),
	MAX_VEL_X(player_data.X_VELOCITY),
	MAX_VEL_Y(player_data.Y_VELOCITY),
	sweepDuration(player_data.SWEEP_DURATION),
	sweepTime(0.0),
	sweepVelX(0.0),
	sweepVelY(0.0),
	kickDuration(player_data.KICK_DURATION),
	kickTime(0.0),
	kickVelX(0.0),
	kickVelY(0.0),
	sprintVelocityMultiplier(player_data.SPRINT_VELOCITY_MULTIPLIER),
	velocityMultiplier(1.0)
{
	log = Log::get_instance();
}

// IMPORTANYE: Proximamente manejar mejor esto con patron State 
void PlayerModel::update(double dt, int x_limit, int y_limit){
    colisionableCounter++;
    if(colisionableCounter<10){
        isColisionable=false;
    }else{
        isColisionable=true;
    }
    using namespace std;

	//PlayerState old_state = this->state;
	// Actualizar x:
	x += velX * velocityMultiplier * dt;
	// Actualizar y:
	y += velY * velocityMultiplier * dt;


	if ((y + this->getHeight()) > y_limit) { //limite de abajo
		y = y_limit - this->getHeight();
	}
	else if (this->y < 0) { // limite de arriba
		this->y = 0;
	}

	if ((x + this->getWidth()) > x_limit) { //limite de abajo
		x = x_limit - this->getWidth();
	}
	else if (this->x < 0) { // limite de arriba
		this->x = 0;
	}

	bool isSweepEnd = false;
	bool isKickEnd = false;

	if (this->state == PlayerState::SWEEPING) {
		this->sweepTime += dt;
		if (this->sweepTime > this->sweepDuration) {
			this->state = PlayerState::STILL;
			isSweepEnd = true;
		}
	}
	
	if (this->state == PlayerState::KICKING) {
		this->kickTime += dt;
		if (this->kickTime > this->kickDuration) {
			this->state = PlayerState::STILL;
			isKickEnd = true;
		}
	}

	if (this->state != PlayerState::SWEEPING && this->state != PlayerState::KICKING) {
		if (velX == 0.0 && velY == 0.0) {
			this->state = STILL;
			// mantenemos el angulo anterior
		}
		else {
			this->state = RUNNING;
			// angle con 0 apunta para arriba, 180 abajo, 360 arriba, lo pasado de 360 o 0 lo modulea SDL2
			this->angle = (atan2(this->velY, this->velX) * 180 / M_PI) + 90;
		}
	}

	if (isSweepEnd) { //seteo las velocidades que me estuve guardando!
		this->velX = sweepVelX;
		this->velY = sweepVelY;
	}

	if (isKickEnd) { //seteo las velocidades que me estuve guardando!
		this->velX = kickVelX;
		this->velY = kickVelY;
	}
}

int PlayerModel::getWidth()
{
	// Esto esta pendiente de mejoras, dependiendo para que se usa esto
	// el tama�o del modelo no es necesariamente igual a un rectangulo como la imagen
	// y no estamos considerando el angulo
	return this->widths[this->state];
}

int PlayerModel::getHeight()
{
	// Esto esta pendiente de mejoras, dependiendo para que se usa esto
	// el tama�o del modelo no es necesariamente igual a un rectangulo como la imagen
	// y no estamos considerando el angulo
	return this->heights[this->state];
}

Team PlayerModel::getTeam()
{
	return this->team;
}

double PlayerModel::getAngle() {
    return angle;

}

PlayerState PlayerModel::getState() {
    return state;

}

int PlayerModel::getKickOff_x() {
    return kickOff_x;
}

int PlayerModel::getKickOff_y() {
    return kickOff_y;
}

double PlayerModel::getVelX() {
    return velX;
}

double PlayerModel::getVelY() {
    return velY;
}

bool PlayerModel::getIsControlledByHuman() {
    return isControlledByHuman;
}

void PlayerModel::setIsControlledByHuman(bool controlled) {
    isControlledByHuman = controlled;
    //Esto es re villero, pero cuando tengamos los 2 equipos recien se va a modificar
    //hasControlOfTheBall = controlled;

}	

double PlayerModel::getMaxVelY() {
    return MAX_VEL_Y;
}

double PlayerModel::getMaxVelX() {
	return MAX_VEL_X;
}

// Proximamente manejar mejor esto con patron State
void PlayerModel::changeVelY(double d)
{

	if (this->state == SWEEPING || this->state == KICKING) {
		sweepVelY += d;
		kickVelY += d;
	}
	else {
		velY += d;
	}
	std::stringstream msg;
	msg << "PlayeModel: cambio en velY, velY=" << velY << "; sweepVelY=" << sweepVelY << "; kickVelY=" << kickVelY;
	log->debug(msg.str());
}

// Proximamente manejar mejor esto con patron State
void PlayerModel::changeVelX(double d)
{
	if (this->state == SWEEPING || this->state == KICKING) {
		sweepVelX += d;
		kickVelX += d;
	}
	else {
		velX += d;
	}
	std::stringstream msg;
	msg << "PlayeModel: cambio en velX, velX=" << velX << "; sweepVelX=" << sweepVelX << "; kickVelX=" << kickVelX;
	log->debug(msg.str());
}

// Proximamente manejar mejor esto con patron State
// Tratar de no usar esto (quizas solo para el controller que devuelva al jugador a su lugar)
void PlayerModel::setVelY(double d) {
	if (this->state == SWEEPING || this->state == KICKING) {
		sweepVelY = d;
		kickVelY = d;
	}
	else {
		velY = d;
	}
	std::stringstream msg;
	msg << "PlayeModel: cambio en velY, velY=" << velY << "; sweepVelY=" << sweepVelY << "; kickVelY=" << kickVelY;
	log->debug(msg.str());
}

// Proximamente manejar mejor esto con patron State
// Tratar de no usar esto (quizas solo para el controller que devuelva al jugador a su lugar)
void PlayerModel::setVelX(double d) {
	if (this->state == SWEEPING || this->state == KICKING) {
		sweepVelX = d;
		kickVelX = d;
	}
	else {
		velX = d;
	}
	std::stringstream msg;
	msg << "PlayeModel: cambio en velX, velX=" << velX << "; sweepVelX=" << sweepVelX << "; kickVelX=" << kickVelX;
	log->debug(msg.str());
}

// Proximamente manejar mejor esto con patron State
void PlayerModel::sweep()
{
	if (this->state != SWEEPING) {
		this->state = SWEEPING;
		this->sweepTime = 0.0;
		this->sweepVelX = velX;
		this->sweepVelY = velY;
	}
	log->debug("PlayerModel: barriendo");
}

// Proximamente manejar mejor esto con patron State
// Parche feo -> Recibe ballModel por ahora // TODO VER
void PlayerModel::kick(BallModel& ballModel,double distance)
{
	if (this->state != KICKING) {
		this->state = KICKING;
		this->kickTime = 0.0;
		this->kickVelX = velX;
		this->kickVelY = velY;
        setHasControlOfTheBall(false);
		ballModel.setAngle(angle);
		ballModel.kick(distance,BallModel::LOW);
	}
	log->debug("PlayerModel: pateando");
}

void PlayerModel::sprint()
{
	this->velocityMultiplier = this->sprintVelocityMultiplier;
	std::stringstream msg;
	msg << "PlayeModel: sprint activado, velocityMultiplier=" << velocityMultiplier;
	log->debug(msg.str());
}

void PlayerModel::stopSprinting()
{
	this->velocityMultiplier = 1.0;
	std::stringstream msg;
	msg << "PlayeModel: sprint desactivado";
	log->debug(msg.str());
}

void PlayerModel::setHasControlOfTheBall(bool control) {
	hasControlOfTheBall=control;
    isColisionable=false;
    colisionableCounter=0;
}

bool PlayerModel::getHasControlOfTheBall() {
    return hasControlOfTheBall;
}

// Parche feo -> Recibe ballModel por ahora // TODO VER
void PlayerModel::changeBallState(BallModel& ballModel) {
	if (hasControlOfTheBall) {
		ballModel.setVelX(getVelX());
		ballModel.setVelY(getVelY());
	}
}

void PlayerModel::pass(PlayerModel *pModel, BallModel& ballModel) {
	int x2=pModel->getCenterX();
	int y2=pModel->getCenterY();
	int x1=ballModel.getX();
	int y1=ballModel.getY();
    double distance=pow(pow(abs(y2-y1),2) + pow(abs(x2-x1),2),0.5);
	double angulo=atan2(y2-y1,x2-x1);
	//std::cout<<std::to_string(angulo)<<std::endl;
    double priorangle=angle;
	angle=(angulo*180/M_PI) + 90;
    setVelX(0);
    setVelY(0);
	//std::cout<<std::to_string(angle)<<std::endl;
	kick(ballModel,distance);
    angle=priorangle;

}

Player_ID PlayerModel::getPlayerId() {
	return this->playerId;
}

void PlayerModel::setAngle(int i) {
	angle = i;
}

void PlayerModel::longPass(PlayerModel *pModel, BallModel &model) {
	int x2=pModel->getCenterX();
	int y2=pModel->getCenterY();
	int x1=model.getX();
	int y1=model.getY();
	double angulo=atan2(y2-y1,x2-x1);
	//std::cout<<std::to_string(angulo)<<std::endl;
	double priorangle=angle;
	angle=(angulo*180/M_PI) + 90;
    double distance=pow(pow(abs(y2-y1),2) + pow(abs(x2-x1),2),0.5);
    //double heightAngle=
	setVelX(0);
	setVelY(0);
	//std::cout<<std::to_string(angle)<<std::endl;
    kickHigh(distance, model);
	angle=priorangle;


}

void PlayerModel::kickHigh(double distance, BallModel &ballModel) {

	if (this->state != KICKING) {
		this->state = KICKING;
		this->kickTime = 0.0;
		this->kickVelX = velX;
		this->kickVelY = velY;
		setHasControlOfTheBall(false);
        ballModel.setAngle(angle);
        ballModel.kick(distance,BallModel::HIGH);
	}
	log->debug("PlayerModel: pateando largo");


}

void PlayerModel::kick(BallModel &model) {
    if (this->state != KICKING) {
        this->state = KICKING;
        this->kickTime = 0.0;
        this->kickVelX = velX;
        this->kickVelY = velY;
        setHasControlOfTheBall(false);
        model.setAngle(angle);
        //double angleToGoal=getAngleToGoal();
        double distanceToGoal=getDistanceToGoal();
        model.kick(distanceToGoal,BallModel::GOAL);
    }
    log->debug("PlayerModel: pateando");

}

double PlayerModel::getAngleToGoal() {
    return 45;
}

double PlayerModel::getDistanceToGoal() {
    return 500;
}
