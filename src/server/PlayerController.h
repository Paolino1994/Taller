//
// Created by federico on 11/04/18.
//

#pragma once

#include "PlayerModel.h"
#include "PlayerView.h"
#include "common/Log.h"
#include "common/Request.h" //pasar a GameConstants.h?
#include "scripted/PlayerControllerScriptedState.h"
#include "common/EventHandler.h"

class PlayerController: public EventHandler
{
protected:
	PlayerModel* playerModel;
	PlayerView* playerView;
	std::unique_ptr<PlayerControllerScriptedState> scriptedState;

	void checkStateChange();
	PlayerController(PlayerModel* model, PlayerView* view, PlayerControllerScriptedState* scriptedState);
public:
	PlayerController(PlayerModel* model, PlayerView* view);
	PlayerController(PlayerController* other);
	virtual ~PlayerController();

	virtual Entity* getEntity();

	virtual PlayerModel* getModel();

	virtual PlayerView* getView();

	// llamar usando new zaraza()!;
	void setScriptedState(PlayerControllerScriptedState* newState);

	// Delegan si hay a un scriptedState:
	void handleEvent(Command &e);
	void update(double dt, int x_limit, int y_limit, int ball_x, int ball_y);
	
	// A redefinit para un controller especifico, funciona si no hay nada scripteado funcionando
	virtual void _handleEvent(Command &e) = 0;
	virtual void _update(double dt, int x_limit, int y_limit, int ball_x, int ball_y) = 0;

	virtual void swap(PlayerController* other);

	virtual bool isControllable() = 0; //isSwappable()

	virtual User_ID getUserId() = 0;
	
	virtual void serialize(player_view_data_t& player_view_data);

    bool hasControlOfTheBall();

    double getAngle();

	// Events to handle:
	virtual void handle(PeriodEndEvent& e);
	virtual void handle(GoalEvent& e);
	virtual void handle(GoalKickEvent& e);
	virtual void handle(GameEndEvent& e);
};
