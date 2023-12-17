
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
#include <poll.h>

#define MY_DOMAIN "0.0.0.0"
#define CONNECTIONS 1000

class Client;
struct pollfd;

class Server
{
	private:
		struct pollfd conn[CONNECTIONS];
		std::string port;
		std::string password;
		int _socket;
		addrinfo *addr;
		std::vector <Client *> clients;
		std::vector <std::string> command;
		Server(void);
	public:
		Server(char **string);
		Server(const Server&);
		Server operator= (const Server&);
		~Server(void);
		void setAddrInfo(void);
		int	getSocket(void);
		std::string	getPort(void);
		void create_socket(void);	
		void bind_socket(void);
		void start_listening(void);
		size_t	findClient(int socket);
		int accept_conn(void);
		int readMsg(int );
		void polling(void);
		void handleCommand(std::vector<std::string>, int, int);
		void privMsgCmd(std::vector<std::string> , int);
		void pingCmd(std::vector<std::string> , int);
		void capLsCmd(std::vector<std::string> , int, int);
		void joinCmd(std::vector<std::string> , int, int);
		std::vector<std::string> parseMsg(std::string );
		std::string getMsg(int socket);
		void run(void);

};

#endif 
