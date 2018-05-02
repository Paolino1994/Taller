#include "controller.h"

int server_controller_main(char* port){
	Socket *svrskt, *cltskt;
	unsigned short s_port = atoi(port);
	char buffer[MAX_MSG];
	int control = SUCCESS;

	svrskt = new Socket();
	if (svrskt->get_status() == ERROR ||
	    svrskt->bind_and_listen(s_port) == ERROR){
		return ERROR;
	}
	
	cltskt = svrskt->accept();
	if (cltskt->get_status() == ERROR) {
		return ERROR;
	}

	do{
		control = cltskt->receive(buffer, MAX_MSG, MSG_NOSIGNAL);
		if (control == ERROR){
			return ERROR;
		}
		printf("%s\n", buffer);
	}while (control != FINISHED && control == MAX_MSG);

	if (cltskt->send(buffer, strlen(buffer), 0) == ERROR) {
		return ERROR;
	}

	delete cltskt;
	delete svrskt;

	return SUCCESS;
}
