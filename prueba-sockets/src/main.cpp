#include <iostream>

#include "server/controller.h"
#include "client/controller.h"


#define SERVER "server"
#define PAR_SRV 3
#define SRV_PRT 2

#define CLIENT "client"
#define PAR_CLT 4
#define CLT_IP 2
#define CLT_PRT	3

int main( int argc, char* argv[] )
{
	if (argc >= PAR_SRV &&
	    strncmp(argv[1], SERVER, strlen(SERVER)) == 0){
		return server_controller_main(argv[SRV_PRT]);
	} else if (argc >= PAR_CLT &&
	    strncmp(argv[1], CLIENT, strlen(CLIENT)) == 0){
		return client_controller_main(argv[CLT_IP], argv[CLT_PRT]);
	} else {
		printf("Wrong command\n");
	}
	return 0;
}
