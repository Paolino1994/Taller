#include <iostream>
#include <vector>
#include <thread>

#include "common/Socket.h"
#include "common/Log.h"
#include "RequestHandler.h"

using namespace std;

void accepter(Socket& skt, bool* exit_requested) {
	vector<RequestHandler*> requestHandlers;
	try {
		while (true) {
			Socket* newClientSocket = skt.accept();
			RequestHandler* rq = new RequestHandler(newClientSocket);
			requestHandlers.push_back(rq);
			rq->run();
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

int main(int argc, char *argv[]) {
	// Ussage: argv[1] = port 
	if (argc < 2) {
		cerr << "Uso: " << argv[0] << " puerto" << endl;
		return 0;
	}
	Log::setFilenamePrefix("server");
	Log::initialize("info");

	Log::get_instance()->info("Iniciamos el servidor...");
	unsigned short port = atoi(argv[1]);

	Socket skt = Socket();
	skt.bind_and_listen(port);
	bool exit_requested = false;
	thread acc(&accepter, ref(skt), &exit_requested);
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

