#include "CommandSender.h"

#include "common/Log.h"
#include "common/Request.h"
#include "common/SocketException.h"
#include "GameManager.h"


CommandSender::CommandSender(std::string ip, unsigned short port):
	protocol(Protocol(ip, port)),
	playerViewData(std::vector<player_view_data_t>()),
	ballViewData({0,0,0,QUIESCENT}),
	gameManagerData({0,0})
{
}


CommandSender::~CommandSender()
{
	// Si falla el shutdown no nos importa mucho ya que igual estamos destruyendo este CommandSender...
	// Medio chancho pero bueno..
	try {
		protocol.shutdown();
	}
	catch (SocketException& ex) {
		Log::get_instance()->error("Error al enviar el shutdown al socket: " + std::string(ex.what()));
	}
	
}

int CommandSender::set_rcv_timeout(time_t seconds){
	return this->protocol.set_rcv_timeout(seconds);
}

void CommandSender::assignTeam(Team team) {
	protocol.write(Request::TEAM_ASSIGN, reinterpret_cast<const char*>(&team), sizeof(team));
}

bool CommandSender::updateModel()
{
	Request request;
	protocol.write(Request::PLAYER_VIEW_UPDATE); // quizas proximamente, le pasamos datos mios de que modelo tengo actualemente u otras yerbas
	protocol.read();

	request = protocol.request();

	if (request == Request::PLAYER_VIEW_UPDATE) {
		const player_view_data_t* player_view_data = reinterpret_cast<const player_view_data_t*>(protocol.dataBuffer());
		size_t player_view_data_len = protocol.dataLength() / sizeof(player_view_data_t);
		playerViewData.clear();
		playerViewData.reserve(player_view_data_len);
		for (size_t i = 0; i < player_view_data_len; ++i) {
			playerViewData.push_back(player_view_data[i]);
		}
	} /*  TODO, no siempre se actualiza
	  else if (request == Request::WAIT){
		this->modelUpdated = false;
		etc....
		return false;
	  }
	  
	  */

	protocol.write(Request::BALL_VIEW_UPDATE); // quizas proximamente, le pasamos datos mios de que modelo tengo actualemente u otras yerbas
	protocol.read();

	request = protocol.request();
	if (request == Request::BALL_VIEW_UPDATE) {
		const ball_view_data_t ball_view_data = *reinterpret_cast<const ball_view_data_t*>(protocol.dataBuffer());
		ballViewData = ball_view_data;
	}/*  TODO, no siempre se actualiza
	  else if (request == Request::WAIT){
		this->modelUpdated = false;
		etc....
		return false;
	  }
	  
	  */

	protocol.write(Request::GAME_MANAGER_UPDATE); // quizas proximamente, le pasamos datos mios de que modelo tengo actualemente u otras yerbas
	protocol.read();

	request = protocol.request();
	if (request == Request::GAME_MANAGER_UPDATE) {
		const game_manager_data_t game_manager_data = *reinterpret_cast<const game_manager_data_t*>(protocol.dataBuffer());
		gameManagerData = game_manager_data;
	}/*  TODO, no siempre se actualiza
	  else if (request == Request::WAIT){
		this->modelUpdated = false;
		etc....
		return false;
	  }
	  
	  */
	
	protocol.write(Request::EVENT_UPDATE); // quizas proximamente, le pasamos datos mios de que modelo tengo actualemente u otras yerbas
	protocol.read();

	request = protocol.request();
	if (request == Request::EVENT_UPDATE) {
		const EventID* event_data = reinterpret_cast<const EventID*>(protocol.dataBuffer());
		size_t event_data_len = protocol.dataLength() / sizeof(EventID);
		events.clear();
		events.reserve(event_data_len);
		for (size_t i = 0; i < event_data_len; ++i) {
			events.push_back(event_data[i]);
		}
	}/*  TODO, no siempre se actualiza*/
	
	return true;
}

model_data_t CommandSender::getModelData()
{
	model_data_t model = { playerViewData, ballViewData, gameManagerData};
	return model;
}

std::vector<EventID>& CommandSender::getEvents()
{
	return this->events;
}

void CommandSender::handleEvent(SDL_Event& e)
{
	Request request = Request::NONE;
	Command command = {CommandKey::__LENGTH__, CommandType::__LENGTH__};

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		request = Request::COMMAND;
		command.key = CommandKey::KEY_DOWN;
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) 
	{
		request = Request::COMMAND;
		command.key = CommandKey::KEY_UP;
	}
		
	if (request != Request::NONE){
		switch (e.key.keysym.sym)
		{
			case SDLK_UP: {
				command.type = CommandType::GO_UP;
				break;
			}
			case SDLK_DOWN: {
				command.type = CommandType::GO_DOWN;
				break;
			}
			case SDLK_LEFT: {
				command.type = CommandType::GO_LEFT;
				break;
			}
			case SDLK_RIGHT: {
				command.type = CommandType::GO_RIGHT;
				break;
			}
			case SDLK_a: {
				command.type = CommandType::SWEEP;
				break;
			}
			case SDLK_SPACE: {
				command.type = CommandType::SPRINT;
				break;
			}
			case SDLK_s: {
				command.type = CommandType::KICK;
				break;
			}
			case SDLK_q: {
				command.type = CommandType::CHANGE_PLAYER;
				break;
			}
			case SDLK_d: {
				command.type = CommandType::PASS;
				break;
			}
			case SDLK_f:{
				command.type = CommandType::LONG_PASS;
			}
		}

		if (command.key != CommandKey::__LENGTH__ && command.type != CommandType::__LENGTH__) {
			protocol.write(request, reinterpret_cast<const char*>(&command), sizeof(command));
		}
	}
}

short CommandSender::login(std::string credentials) {
	protocol.write(Request::LOGIN, credentials);
	protocol.read();
	if (protocol.request() == Request::LOGIN) {
		int result = std::stoi(protocol.dataBuffer());
		if (result == USER_ACCEPTED) {
			return LOGIN_SUCCESS;
		} else {
			return result;
		}
	} else {
		return LOGIN_ERROR;
	}
}

Request CommandSender::listenStart() {
	protocol.write(Request::START);
	protocol.read();
	return protocol.request();
}

bool CommandSender::checkFormation(Team team) {
	protocol.write(Request::CHECK_TEAM_FORMATION, reinterpret_cast<const char*>(&team), sizeof(team));
		protocol.read();
		Request request;
		request = protocol.request();
		if (request == Request::TEAM_WITH_FORMATION) {
			return true;
		}else{
			return false;
		}
}

void CommandSender::setTeamFormation(Team team, Formation formation) {
	TeamFormation teamFormation = {Team::__LENGTH__, Formation::__LENGTH__};

	teamFormation.team = team;
	teamFormation.formation = formation;

	protocol.write(Request::SET_TEAM_FORMATION, reinterpret_cast<const char*>(&teamFormation), sizeof(teamFormation));
}
