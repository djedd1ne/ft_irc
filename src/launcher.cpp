
#include "../inc/Input.hpp"
#include "../inc/Server.hpp"
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

int main(int argc, char **argv)
{
	//do memory management before exits in Server (if exists);
	//change perror to trow exceptions;

	struct addrinfo		*test;
	Server server;
	int socket;
	int status;
	(void)status;
    int port = atoi(argv[1]);
	std::string port_s(argv[1]);
    std::string password = argv[2];
	std::string	ip_s("127.0.0.1");
	
    if (argc != 3) {
        std::cerr << "Usage: ./server <port> <pass>" << std::endl;
        return 1;
    }

	status = getaddrinfo(ip_s.c_str(), port_s.c_str(), NULL, &test);
	socket = server.create_socket();
	server.bind_socket(socket, &test);
	server.start_listening(socket);

    std::cout << "Server is listening on port " << port << std::endl;

    while (1)
	{
		server.accept_conn(socket);
	}
}
