
#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>

#define MY_DOMAIN "0.0.0.0"

struct info 
{
	std::string name;
	std::string value;
};

struct hdata 
{
	std::string name;
	std::string value;
};

class Server
{
	private:
	public:
		Server(void);
		Server(const Server&);
		Server operator= (const Server&);
		~Server(void);
		int create_socket(void);	
		void bind_socket(int , addrinfo **);
		void start_listening(int );
		int accept_conn(int , addrinfo **);
		int read_messages(int socket);
		void send_messages(int socket);

};

#endif
