
#include "../inc/Input.hpp"
#include <stdio.h>
#include <netinet/in.h>

// Constructors

Input::Input(int argc, char **argv)
{
	if (argc == 3)
	{
		this->port = argv[1];
		this->password = argv[2];
	}
	else 
	{
        std::cerr << "Usage: ./server <port> <pass>" << std::endl;
		exit(1);
	}
}

Input::Input(const Input &src)
{
	this->port = src.port;	
	this->password = src.password;	
}

Input Input::operator= (const Input &src)
{
	this->port = src.port;	
	this->password = src.password;	
	return (*this);
}

Input::~Input(void)
{
}

// Functions

void Input::parseInput(void)
{
	int portNo;

	for (int i = 0; i < (int)port.size(); i++)
	{
		if (!isdigit(port[i]))
		{
			std::cerr << "Port not specified correctly!" << std::endl;
			exit(1);
		}
	}
	portNo = atoi(port.c_str());
	if (portNo < 1024 || portNo > 9000)
	{
		std::cerr << "Port our of range, pick between 1024 and 9000!" << std::endl;
		exit(1);
	}
	if (this->password.size() > 13 || this->password.size() < 4)
	{
		std::cerr << "Password must be between 4 and 13 characters!" << std::endl;
		exit(1);
	}
}

void Input::getAddrInfoStruct(addrinfo **addr)
{
	getaddrinfo(MY_DOMAIN, this->port.c_str(), NULL, addr);
}

std::string Input::getPort(void)
{
	return (this->port);
}
