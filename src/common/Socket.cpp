#include "Socket.h"
#include "SocketException.h"

int Socket::control_output(int control){
	if (control == SKT_ERROR){
		throw SocketException();
	}
	return SKT_SUCCESS;
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
	int control = close(fd);
	status = control_output(control);
}

int Socket::set_rcv_timeout(time_t seconds){
    	int control;
    	struct timeval timeout;
    	timeout.tv_sec = seconds; //segundos
    	timeout.tv_usec = 0; //microsegundos
   	control = setsockopt(this->fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    	status = control_output(control);
	return status;
}



int Socket::connect(const char* ip, unsigned short port){
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
	if (control != SKT_ERROR){
        	control = listen(fd, SKT_BACKLOG);
	}
	status = control_output(control);
    return status;
}

Socket* Socket::accept(){
	int client_fd = ::accept(fd, 0, 0);
	status = control_output(client_fd);
	if (status == SKT_ERROR)
		return NULL;
	Socket* client = new Socket(client_fd);
	return client;
}

int Socket::shutdown(){
	int control = ::shutdown(fd, SHUT_RDWR);
	status = control_output(control);
	return status;
}

int Socket::send(const char* msg, u_int32_t len, int flags) {
	int bytes = SOCKET_OPENED;
	u_int32_t bytes_sent = 0;
	while (bytes_sent < len && bytes != SKT_ERROR && bytes != SOCKET_CLOSED){
		bytes = ::send(fd, msg + bytes_sent, len - bytes_sent, flags);
		if (bytes > 0){
			bytes_sent += bytes;
		}
	}
	status = control_output(bytes == SOCKET_CLOSED ? SKT_ERROR : bytes);
	return (bytes == SKT_FINISHED)? SKT_FINISHED : control_output(bytes);
}

int Socket::receive(char* buff, u_int32_t len, int flags){
	int bytes = SOCKET_OPENED;
	u_int32_t bytes_recv = 0;
	while(bytes_recv < len && bytes != SKT_ERROR && bytes != SOCKET_CLOSED){
		bytes = ::recv(fd, buff + bytes_recv, len - bytes_recv,
			flags);
		if (bytes > 0){
			bytes_recv += bytes;
		}
	}
	status = control_output(bytes == SOCKET_CLOSED? SKT_ERROR: bytes);
	if (bytes == SKT_ERROR){
		return control_output(bytes);
	}
	return (bytes == SOCKET_CLOSED && bytes_recv == 0)? SKT_FINISHED : bytes_recv;
}

int Socket::get_status() {
	return status;
}
