#pragma once

#include <vector>
#include <string>

#include "Request.h"
#include "Socket.h"

struct header_t
{
	Request request;
	u_int32_t len;
};

class Protocol
{
public:
	Protocol(Socket*); //Server-side
	Protocol(std::string ip, unsigned short port); //Client-side
	~Protocol();

	void shutdown();

	Request request();
	size_t dataLength();
	const char* dataBuffer();

	void read();
	void write(Request request, const char* data, u_int32_t len);
	inline void write(Request request, const std::string& message) {
		this->write(request, message.c_str(), message.length());
	};

private:
	Socket* skt; //suponemos que hay que hacerle delete
	std::vector<char> buffer;
	header_t header;
};

