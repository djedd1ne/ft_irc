
#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>

#define MY_DOMAIN "0.0.0.0"

class Client;
struct pollfd;

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
		char *port;
		int _socket;
		addrinfo *addr;
		std::vector <Client *> clients;
		Server(void);
	public:
		Server(char *string);
		Server(const Server&);
		Server operator= (const Server&);
		~Server(void);
		void setAddrInfo(void);
		int	getSocket(void);
		void create_socket(void);	
		void bind_socket(void);
		void start_listening(void);
		int accept_conn(void);
		int read_messages(int );
		void send_messages(int );
		void registerClient(int );

};

#endif
