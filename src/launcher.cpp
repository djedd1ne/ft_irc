
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

#define CONNECTIONS 10

int main(int argc, char **argv)
{
	//do memory management before exits in Server (if exists);
	//change perror to trow exceptions;

	int			socket;
	Input		input(argc, argv);
	Server		server;
	addrinfo	*test;
	pollfd		conn[CONNECTIONS];
	int			existingConns;

	(void)conn;
	input.parseInput();
	input.getAddrInfoStruct(&test);

	socket = server.create_socket();
	server.bind_socket(socket, &test);

    std::cout << "Server is listening on port " << input.getPort() << std::endl;

	int clientsocket;
	server.start_listening(socket);

	conn[0].fd = socket;
	conn[0].events = POLLIN;
	existingConns = 1;
	while(1)
	{
		poll(conn, CONNECTIONS, 200);
		//iterate over conns
		for (int i = 0; i < existingConns; i++)
		{
			if (conn[i].revents & POLLIN)
			{
				if (conn[i].fd == socket)
				{
					clientsocket = server.accept_conn(socket, &test);
					conn[i].fd = clientsocket;
					conn[i].events = POLLIN;
					existingConns++;
					std::cout << "New connection accepted: "<<std::endl;
				}
				else
				{
					server.read_messages(conn[i].fd);
				}
			}
		}
	}
}
