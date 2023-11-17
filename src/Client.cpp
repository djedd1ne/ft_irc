
#include "../inc/Client.hpp"

// Constructors

Client::Client(void)
{
}

Client::Client(int &sock)
{
	socket = sock;	
	registered = false;
	op = false;
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

int Client::getSocket(void)
{
	return (this->socket);
}

bool Client::isRegistered()
{
	return (registered);
}

bool Client::isOp()
{
	return (op);
}

void Client::registerUser()
{
	registered = true;
}
