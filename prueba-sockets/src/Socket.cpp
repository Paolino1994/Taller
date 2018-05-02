#include "Socket.h"

int Socket::control_output(int control){
        if (control == ERROR){
                printf("Error: %s\n", strerror(errno));
                return ERROR;
        }
        return SUCCESS;
}

Socket::Socket(){
	fd = socket(AF_INET, SOCK_STREAM, 0);
	status = control_output(fd);
}

Socket::Socket(int fd){
	this->fd = fd;
	status = control_output(fd);
}

Socket::~Socket(){
	shutdown(SHUT_RDWR);
	int control = close(fd);
	status = control_output(control);
}

int Socket::connect(char* ip, unsigned short port){
	int control;
	struct sockaddr_in sktaddr;
	sktaddr.sin_family = AF_INET;
	sktaddr.sin_port = htons(port);
	sktaddr.sin_addr.s_addr = ::inet_addr(ip);
	memset(sktaddr.sin_zero, 0, sizeof(sktaddr.sin_zero));

	control = ::connect(fd, (struct sockaddr *) &sktaddr,
		sizeof(struct sockaddr_in));
	status = control_output(control);
	return status;
}

int Socket::bind_and_listen(unsigned short port){
	int control;
	struct sockaddr_in sktaddr;
        sktaddr.sin_family = AF_INET;
        sktaddr.sin_port = htons(port);
        sktaddr.sin_addr.s_addr = INADDR_ANY;
	memset(sktaddr.sin_zero, 0, sizeof(sktaddr.sin_zero));

        control = ::bind(fd, (struct sockaddr *) &sktaddr,
                sizeof(struct sockaddr_in));
	if (control != ERROR){
        	control = listen(fd, BACKLOG);
	}
	status = control_output(control);
    return status;
}

Socket* Socket::accept(){
	int client_fd = ::accept(fd, 0, 0);
	status = control_output(client_fd);
	if (status == ERROR)
		return NULL;
	Socket* client = new Socket(client_fd);
	return client;
}

int Socket::shutdown(int mode){
	int control = ::shutdown(fd, mode);
	status = control_output(control);
	return status;
}

int Socket::send(char* msg, short len, int flags){
	int bytes_sent = 0, bytes = SOCKET_OPENED;
	while (bytes_sent < len && bytes != ERROR && bytes != SOCKET_CLOSED){
		bytes = ::send(fd, msg + bytes_sent, len - bytes_sent, flags);
		if (bytes > 0){
			bytes_sent += bytes;
		}
	}
	status = control_output(bytes);
	return (bytes == FINISHED)? FINISHED : control_output(bytes);
}

int Socket::receive(char* buff, short len, int flags){
	int bytes_recv = 0, bytes = SOCKET_OPENED;
	while(bytes_recv < len && bytes != ERROR && bytes != SOCKET_CLOSED){
		bytes = ::recv(fd, buff + bytes_recv, len - bytes_recv,
			flags);
		if (bytes > 0){
			bytes_recv += bytes;
		}
	}
	status = control_output(bytes);
	if (bytes == ERROR){
		return control_output(bytes);
	}
	return (bytes == SOCKET_CLOSED && bytes_recv == 0)? FINISHED : bytes_recv;
}

int Socket::get_status() {
	return status;
}
