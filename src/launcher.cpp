
#include "../inc/Input.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

//do memory management before exits in Server (if exists);
//change perror to trow exceptions;

int main(int argc, char **argv)
{
	Input		input(argc, argv);
	Server		server(argv);

	input.parseInput();
	server.run();
	return (EXIT_SUCCESS);
}
