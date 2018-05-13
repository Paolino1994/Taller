#include "Protocol.h"


Protocol::Protocol(Socket* socket):
	skt(socket),
	buffer(std::vector<char>()),
	header({ Request::NONE,0 })
{
}

Protocol::Protocol(std::string ip, unsigned short port):
	Protocol(new Socket())
{
	this->skt->connect(ip.c_str(), port);
}

Protocol::~Protocol()
{
	if (!skt_protected) {
		delete this->skt;
	}
}


void Protocol::shutdown()
{
	this->skt->shutdown();
}

Request Protocol::request()
{
	return this->header.request;
}

size_t Protocol::dataLength()
{
	return this->header.len;
}

const char * Protocol::dataBuffer()
{
	return this->buffer.data();
}

void Protocol::read()
{
	skt->receive((char*)&header, sizeof(header));
	if (header.len > 0) {
		buffer.reserve(header.len);
		skt->receive(buffer.data(), header.len);
	}
}

void Protocol::write(Request request, const char * data, u_int32_t len)
{
	header_t writeHeader{ request, len };
	skt->send((char*)&writeHeader, sizeof(writeHeader), Socket::DEFAULT_FLAGS | MSG_MORE);
	skt->send(data, len);
}

void Protocol::write(Request request)
{
	header_t writeHeader{ request, 0 };
	skt->send((char*)&writeHeader, sizeof(writeHeader));
}

void Protocol::protect() {
	skt_protected = true;
}
