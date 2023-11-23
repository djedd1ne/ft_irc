
#include "../inc/Input.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
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
	server.start_listening();
	server.polling();

}
