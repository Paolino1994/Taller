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

int Protocol::set_rcv_timeout(time_t seconds){
	return this->skt->set_rcv_timeout(seconds);
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

std::string Protocol::dataAsString()
{
	return std::string(this->dataBuffer(), this->dataLength());
}

void Protocol::read()
{
	skt->receive((char*)&header, sizeof(header));
	if (header.len > 0) {
		buffer.reserve(header.len);
		skt->receive(buffer.data(), header.len);
	}
}

void Protocol::readText()
{
	skt->receive((char*)&header, sizeof(header));
	if (header.len > 0) {
		buffer.reserve(header.len + 1);
		int len = skt->receive(buffer.data(), header.len);
		buffer[len] = '\0';
	}
}

void Protocol::write(Request request, const char * data, u_int32_t len)
{
	if (len == 0) {
		this->write(request);
	}
	else {
		header_t writeHeader{ request, len };
		skt->send((char*)&writeHeader, sizeof(writeHeader), Socket::DEFAULT_FLAGS | MSG_MORE);
		skt->send(data, len);
	}
}

void Protocol::write(Request request)
{
	header_t writeHeader{ request, 0 };
	skt->send((char*)&writeHeader, sizeof(writeHeader));
}

void Protocol::protect() {
	skt_protected = true;
}

bool Protocol::hasSkt(Socket* s) {
	return s == skt;
}
