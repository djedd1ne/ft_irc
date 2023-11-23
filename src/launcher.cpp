
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

int main(int argc, char **argv)
{
	//do memory management before exits in Server (if exists);
	//change perror to trow exceptions;

	Input		input(argc, argv);
	Server		server(argv);

	input.parseInput();
	server.setAddrInfo();

	server.create_socket();
	server.bind_socket();

    std::cout << "Server is listening on port " << input.getPort() << std::endl;

	server.start_listening();
	server.polling();

}
