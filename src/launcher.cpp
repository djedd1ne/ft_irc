
#include "../inc/Input.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
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
#include <vector>

int main(int argc, char **argv)
{
	//do memory management before exits in Server (if exists);
	//change perror to trow exceptions;

	int			socket;
	Input		input(argc, argv);
	Server		server;
	addrinfo	*test;

	
	input.parseInput();
	input.getAddrInfoStruct(&test);

	socket = server.create_socket();
	server.bind_socket(socket, &test);
	server.start_listening(socket);

    std::cout << "Server is listening on port " << input.getPort() << std::endl;

	int clientsocket;
	std::vector <Client*> clients;

	(void)clients;
    while (1)
	{
		clientsocket = server.accept_conn(socket, &test);
		Client new_client(clientsocket);
		clients.push_back(&new_client);
		printf("read msg\n");
		server.read_messages(clientsocket);
	}
}
