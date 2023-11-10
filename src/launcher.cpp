
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
#include <sys/time.h>
#include <vector>
#include <poll.h>

int main(int argc, char **argv)
{
	//do memory management before exits in Server (if exists);
	//change perror to trow exceptions;

	int			socket;
	Input		input(argc, argv);
	Server		server;
	addrinfo	*test;
	pollfd	conn;

	(void)conn;
	input.parseInput();
	input.getAddrInfoStruct(&test);

	socket = server.create_socket();
	server.bind_socket(socket, &test);

    std::cout << "Server is listening on port " << input.getPort() << std::endl;

	int clientsocket;

    while (1)
	{
		server.start_listening(socket);
		clientsocket = server.accept_conn(socket, &test);
		server.read_messages(clientsocket);
	}
}
