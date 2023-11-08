
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
		void accept_conn(int , addrinfo **);

};

#endif
