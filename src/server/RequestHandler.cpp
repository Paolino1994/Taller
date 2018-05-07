#include "RequestHandler.h"

#include "common/Log.h"

const std::string& getDescription(Command command) {
	return commandDescription[static_cast<std::underlying_type<Command>::type>(command)];
}

void RequestHandler::_run()
{
	try {
		while (true) {
			protocol.read();

			Request request = protocol.request();
			switch (request) {
				case Request::COMMAND_KEY_DOWN: {
					Log::get_instance()->info("Me llego un comando del tipo KEY_DOWN");
					Command command = *reinterpret_cast<const Command*>(protocol.dataBuffer());
					Log::get_instance()->info("El comando es: " + getDescription(command));
					break;
				}
				case Request::COMMAND_KEY_UP: {
					Log::get_instance()->info("Me llego un comando del tipo KEY_UP");
					Command command = *reinterpret_cast<const Command*>(protocol.dataBuffer());
					Log::get_instance()->info("El comando es: " + getDescription(command));
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
		}
	}
	running = false;
	if (!this->server_exit_requested) protocol.shutdown(); //Por problemas con la conexion con este cliente
}

RequestHandler::RequestHandler(Socket * socket) :
	protocol(Protocol(socket)),
	running(false),
	server_exit_requested(false)
{
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
