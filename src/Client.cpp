
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
	_capSent = false;
	_passSent = false;
	_nickSent = false;
	_usrSent = false;
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

void Client::capSent()
{
	_capSent = true;
}

bool Client::wasCapSent()
{
	return (_capSent);
}

void Client::setNick(std::string nick)
{
	_nick = nick;
}

void Client::setUsername(std::string username)
{
	_username = username;
}

void Client::nickSent()
{
	_nickSent = true;
}

void Client::passSent()
{
	_passSent = true;
}

void Client::userSent()
{
	_usrSent = true;
}

bool Client::wasNickSent()
{
	return (_nickSent);
}

bool Client::wasPassSent()
{
	return (_passSent);
}

bool Client::wasUSerSent()
{
	return (_usrSent);
}
