
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include <fcntl.h>

// Constructors

Client::Client(void)
{
	registered = false;
	op = false;
	_capSent = false;
	_passSent = false;
	_nickSent = false;
	_usrSent = false;
	clientAddrLen = sizeof(clientAddr);
}

Client::Client(const Client &src)
{
	(void) src;
}

Client Client::operator= (const Client &src)
{
	(void)src;
	_socket = src._socket;
	return (*this);
}

Client::~Client(void)
{
    struct sockaddr_in *ptr = (struct sockaddr_in *)&clientAddr;
	std::cout << "Client left :: "  << inet_ntoa(ptr->sin_addr)<<std::endl;
}

// Functions

void Client::addChannel(Channel *channel)
{
	std::cout<<"added channel -"<<channel->getName()<<std::endl;
	userChannels.push_back(channel);
	std::cout<<"channel test: "<<userChannels[0]->getName()<<std::endl;
}

void Client::setSocket(int &sock)
{
	_socket = sock;
}

int Client::acceptConnection(int &socket)
{
	_socket = accept(socket, (sockaddr *)&clientAddr, &clientAddrLen);
	if (_socket == -1) 
	{
		throw("Failed to accept incoming connection");
		close(_socket);
		exit(1);
    }
	fcntl(_socket, F_SETFL, O_NONBLOCK);
//	else
	{
        struct sockaddr_in *ptr = (struct sockaddr_in *)&clientAddr;
		std::cout << "ACCEPTED :: "  << inet_ntoa(ptr->sin_addr)<<std::endl;
		_ip = inet_ntoa(ptr->sin_addr);

	}
	return (this->_socket);
}

int Client::getSocket(void)
{
	return (this->_socket);
}

struct sockaddr_storage* Client::getClientAddr(void)
{
	return (clientAddr);
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

std::string Client::getIp(void)
{
	return (this->_ip);
}

std::string Client::getNick(void)
{
	return (this->_nick);
}

std::string Client::getUsername(void)
{
	return (this->_username);
}

Channel* Client::getChannel(void)
{
	return (this->userChannels[0]);
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
