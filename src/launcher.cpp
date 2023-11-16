
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

	Input		input(argc, argv);
	Server		server(argv[1]);
	pollfd		conn[CONNECTIONS];
	int			existingConns;

	(void)conn;
	input.parseInput();
	server.setAddrInfo();

	server.create_socket();
	server.bind_socket();

    std::cout << "Server is listening on port " << input.getPort() << std::endl;

	server.start_listening();

	int pollc;
	conn[0].fd = server.getSocket();
	conn[0].events = POLLIN;
	existingConns = 1;
	while(1)
	{
		pollc = poll(conn, existingConns, -1);
		(void)pollc;
		//iterate over conns
		for (int i = 0; i < existingConns; i++)
		{
			// if file descriptor is ready to read
			if (conn[i].revents & POLLIN)
			{
				//if server is ready to read, handle new conn
				if (conn[i].fd == server.getSocket())
				{
					conn[existingConns].fd = server.accept_conn();
					server.registerClient(conn[existingConns].fd);
					conn[existingConns].events = POLLIN;
					existingConns++;
					
					std::cout << "New connection accepted: "<<std::endl;
				}
				//if not listener then its just a regular client
				else
				{
					if (server.read_messages(conn[i].fd) == 0)
					{
						printf("close it \n");
						close(conn[i].fd);
						printf("before\n");
						conn[i] = conn[existingConns - 1];
						printf("after\n");
						existingConns--;
					}
				}
			}
		}
	}
}
