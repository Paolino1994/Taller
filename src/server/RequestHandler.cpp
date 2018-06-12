#include "RequestHandler.h"

#include "common/Log.h"
#include <iostream>

const std::string& getDescription(CommandType command) {
	return commandTypeDescription[static_cast<std::underlying_type<CommandType>::type>(command)];
}

void RequestHandler::_run()
{
	try {
		while (true) {
			protocol.read();

			Request request = protocol.request();
			switch (request) {
				case Request::TEAM_ASSIGN: {
					Team team = *reinterpret_cast<const Team*>(protocol.dataBuffer());
					Log::get_instance()->info("Me llego un pedido de asignacion al equipo: " + static_cast<std::underlying_type<Team>::type>(team));
					player = game.assignToTeam(team, this->userId);
					break;
				}
				case Request::COMMAND: { 
					Command command = *reinterpret_cast<const Command*>(protocol.dataBuffer());
					game.handleCommandForPlayer(this->player, command);
					break;
				}
				case Request::MODEL_UPDATE: {
					if (this->modelSnapshotNumber == this->game.getModelSnapshotNumber()) {
						protocol.write(Request::WAIT);
					}
					else {
						protocol.write(Request::MODEL_UPDATE); // responder con el nro de snapshot?
						this->modelSnapshotNumber = this->game.getModelSnapshotNumber();
						this->game.copyModelData(this->modelData);
					}
					break;
				}
				case Request::PLAYER_VIEW_UPDATE: {
					protocol.write(Request::PLAYER_VIEW_UPDATE, (const char*)this->modelData.playerViewData.data(), sizeof(player_view_data_t) * this->modelData.playerViewData.size());
					break;
				}
				case Request::BALL_VIEW_UPDATE: {
					protocol.write(Request::BALL_VIEW_UPDATE, (const char*)&this->modelData.ballViewData, sizeof(this->modelData.ballViewData));
					break;
				}
				case Request::GAME_MANAGER_UPDATE: {
					protocol.write(Request::GAME_MANAGER_UPDATE, (const char*)&this->modelData.gameManagerData, sizeof(this->modelData.gameManagerData));
					break;
				}
				case Request::EVENT_UPDATE: {
					std::vector<EventID> eventsToSend = std::vector<EventID>();
					eventsToSend.swap(this->events); // asi this->events queda vacio y mando los que tenia
					protocol.write(Request::EVENT_UPDATE, (const char*)eventsToSend.data(), sizeof(EventID) * eventsToSend.size());
					break;
				}
				case Request::START: {
					bool started = false;
					(UserManager::get_instance()).user_ready(protocol);
					do {
						if ((UserManager::get_instance()).game_started()) {
							started = true;
							protocol.write(Request::START);
						} else {
							std::this_thread::sleep_for (std::chrono::seconds(2));
						}
					} while (! started);
					protocol.set_rcv_timeout(30);
					break;
				}
				case Request::CHECK_TEAM_FORMATION: {
					Team team = *reinterpret_cast<const Team*>(protocol.dataBuffer());
					Log::get_instance()->info("se comprueba si existe formación del equipo: " + static_cast<std::underlying_type<Team>::type>(team));
					bool result = game.checkTeamFormation(team, this->userId);
					if(result) protocol.write(Request::TEAM_WITH_FORMATION);
						else protocol.write(Request::TEAM_WITHOUT_FORMATION);
					break;
				}
				case Request::SET_TEAM_FORMATION: {
					TeamFormation teamFormation = *reinterpret_cast<const TeamFormation*>(protocol.dataBuffer());
					game.setTeamFormation(teamFormation.team, teamFormation.formation, this->userId);
					break;
				}
				default: {
					Log::get_instance()->info("Unhandled request");
					break;
				}

			}
		}
	}
	catch (std::exception& e) {
		if (this->server_exit_requested) {
			Log::get_instance()->info("Finalizo la conexion por pedido del servidor");
		}
		else {
			Log::get_instance()->error("Conexion con el cliente finalizo de la sig. forma: " + std::string(e.what()));
			UserManager& u_manager = UserManager::get_instance();
			u_manager.logout(protocol);
		}
	}
	running = false;
	if (!this->server_exit_requested) protocol.shutdown(); //Por problemas con la conexion con este cliente

	// Me voy del juego!
	if (this->game.withdrawUser(this->player, this->userId)) {
		this->player = nullptr;
	}
	else {
		Log::get_instance()->error("Al desconectar a un cliente, no pudimos expulsar su controlador del juego");
	}
	// este objeto queda vivo pero no tiene sentido seguir recibiendo eventos:
	this->unRegisterFromAllEvents();
}

RequestHandler::RequestHandler(Socket * socket, Game& game, User_ID userId) :
	protocol(Protocol(socket)),
	game(game),
	userId(userId),
	running(false),
	server_exit_requested(false),
	events(std::vector<EventID>()),
	player(nullptr)
{
	this->registerToAllEvents();
	this->game.copyModelData(this->modelData);
	this->modelSnapshotNumber = this->game.getModelSnapshotNumber();
}

RequestHandler::~RequestHandler()
{
	if (this->running) { // me estan terminando desde afuera, es decir el servidor quiere cerrar
		this->server_exit_requested = true;
		protocol.shutdown(); //deberia desbloquear el thread
	}
	worker.join();
}

void RequestHandler::run()
{
	if (!running) {
		worker = std::thread(&RequestHandler::_run, this);
		running = true;
	}
}

void RequestHandler::handleFallback(Event & e)
{
	this->events.push_back(e.getId());
}
