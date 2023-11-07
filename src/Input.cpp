#include "../inc/Input.hpp"
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>

// Constructors

Input::Input(int argc, char **argv)
{
	char *temp;

	if (argc == 3)
	{
		this->password = strdup(argv[2]);
		temp = strdup(argv[1]);
		this->port = atoi(temp);
		free(temp);
	}
	else 
		this->password = NULL;
}

Input::Input(const Input &src)
{
	this->password = src.password;	
}

Input Input::operator= (const Input &src)
{
	this->password = src.password;	
	this->port = src.port;	
	return (*this);
}

Input::~Input(void)
{
	if (password != NULL)
		free(this->password);
}

// Functions

void Input::parse(sockaddr_in &addr)
{
	addr.sin_port = htons(port);

	printf("port: %d\n", port);
	printf("converted port: %d\n", addr.sin_port);
	printf("pass: %s\n", password);
}
