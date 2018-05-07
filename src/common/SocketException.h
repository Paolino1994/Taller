#pragma once

#include <string>
#include <stdexcept>

class SocketException : public std::runtime_error
{
private:
	std::string errorMessage;
public:
	SocketException() throw();
	virtual char const* what() const throw();
};

