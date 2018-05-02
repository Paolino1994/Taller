#include "controller.h"

int client_controller_main(char* server, char* port){
	Socket* skt;
	unsigned short s_port = atoi(port);
	char buffer[] = "Hola Mundo!";
	int control = MAX_MSG;
	
	skt = new Socket();
	if (skt->get_status() == ERROR ||
	    skt->connect(server, s_port) == ERROR){
		return ERROR;
	}

	while (control == MAX_MSG){
		control = strlen(buffer);
		if (skt->send(buffer, control, 0) == ERROR){
			return ERROR;
		}
	}

	if (skt->shutdown(SHUT_WR) == ERROR ||
	    (control = skt->receive(buffer, MAX_MSG, MSG_NOSIGNAL)) == ERROR){
		return ERROR;
	}
	buffer[control] = '\0';
	printf("%s\n", buffer);

	delete skt;
/*	if (skt && skt->get_status() == ERROR){
		return ERROR;
	}
*/
	return SUCCESS;
}
