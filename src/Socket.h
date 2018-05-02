#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BACKLOG 10
#define ERROR -1
#define SUCCESS 0
#define FINISHED -5
#define SOCKET_CLOSED 0
#define SOCKET_OPENED 1

class Socket {
private:
	int fd;

	int status;

	int control_output(int control);

public:
	Socket();

	Socket(int fd);

	~Socket();

	int connect(char* ip, unsigned short port);

	int bind_and_listen(unsigned short port);

	Socket* accept();

	int shutdown(int mode);

	int send(char* msg, short len, int flags);

	int receive(char* buff, short len, int flags);

	int get_status();
};

#endif
