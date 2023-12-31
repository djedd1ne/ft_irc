
#ifndef INPUT_HPP
#define INPUT_HPP

#include <string>
#include <iostream>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>

#define MY_DOMAIN "0.0.0.0"

class Input
{
	private:
		std::string port;
		std::string	password;
	public:
		Input(int, char **);
		Input(const Input&);
		Input operator= (const Input&);
		~Input(void);
		void getAddrInfoStruct(addrinfo **);
		void parseInput(void);
		std::string getPort(void);

};

#endif
