#include <iostream>
#include <vector>
#include <thread>

#include "common/Socket.h"
#include "common/Log.h"
#include "RequestHandler.h"
#include "UserManager.h"
#include "Game.h"

using namespace std;

void accepter(Game& game, Socket& skt, bool* exit_requested) {
	User_ID userId = 1;
	vector<RequestHandler*> requestHandlers;
	UserManager& u_manager = UserManager::get_instance();
	try {
		while (true) {
			Socket* newClientSocket = skt.accept();
			if (u_manager.login(newClientSocket) == LOGIN_SUCCESS) {
				// Inyecto un jugador controlado por un humano
				RequestHandler* rq = new RequestHandler(newClientSocket, game, userId);
				requestHandlers.push_back(rq);
				rq->run();
				userId++;
			} else {
				delete newClientSocket;
			}
		}
	}
	catch (exception& e) {
		if (*exit_requested) {
			Log::get_instance()->info("Me pidieron salir desde la terminal, cierro todas las conexiones.");
		}
		else {
			Log::get_instance()->info("Error: Servidor finalizo de la sig. forma: " + string(e.what()));
		}
	}
	for (auto rq: requestHandlers)
	{
		delete rq;
	}
}

std::string getLogType(std::string str) {
    if( str.compare(LOG_ERROR)==0 || str.compare(LOG_DEBUG)==0 || str.compare(LOG_INFO)==0){
        return str;
    }
    return LOG_ERROR;
}


int main(int argc, char *argv[]) {
	// Ussage: argv[1] = port 
	if (argc < 2) {
		cerr << "Uso: " << argv[0] << " puerto" << endl;
		return 0;
	}

    std::string yamlConfigFile = "";
	Log::setFilenamePrefix(LOG_SERVER);
	for (int i = 2; i+1 < argc; i++) {
		if (strcmp(argv[i],"-lg") == 0) {
            std::string logType=getLogType(argv[i+1]);
            Log::initialize(logType);
            Log::get_instance()->error("Log cargado en modo " + logType);
        } else if (strcmp(argv[i],"-yaml") == 0) {
            yamlConfigFile = argv[i+1];
        }
	}
	YAMLReader& yamlReader = YAMLReader::get_instance();
	yamlReader.readYamlGeneral(yamlConfigFile);
	if (!Log::is_initialized()) {
        std::string logType = yamlReader.getLogLevel();
        logType = getLogType(logType);
        Log::initialize(logType);
        Log::get_instance()->error("Log cargado en modo " + logType);
    }

	Log::get_instance()->info("Iniciamos el servidor...");
	unsigned short port = atoi(argv[1]);

	Socket skt = Socket();
	skt.bind_and_listen(port);
	bool exit_requested = false;

	Game game;

	thread acc(&accepter, ref(game), ref(skt), &exit_requested);
	Log::get_instance()->info("Estamos aceptando clientes");
	while (true) {
		cout << "Ingrese \"*\" para finalizar: ";
		string s;
		getline(cin, s);
		if (s == "*") break;
	}
	exit_requested = true;
	skt.shutdown(); //desbloquea el accepter
	acc.join();
	return 0;
}
