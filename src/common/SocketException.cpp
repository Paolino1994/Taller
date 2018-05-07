#include "SocketException.h"

#include <errno.h>
#include <string.h>

SocketException::SocketException() throw(): std::runtime_error("SocketException"),
	errorMessage(strerror(errno))
{
}

char const * SocketException::what() const throw()
{
	return this->errorMessage.c_str();
}

