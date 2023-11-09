
#include "../inc/Client.hpp"

// Constructors

Client::Client(void)
{
}

Client::Client(int &sock)
{
	socket = sock;	
}

Client::Client(const Client &src)
{
	(void) src;
}

Client Client::operator= (const Client &src)
{
	(void)src;
	return (*this);
}

Client::~Client(void)
{
}

// Functions

void Client::setSocket(int &sock)
{
	socket = sock;
}
