
#include "../inc/Input.hpp"
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
	sockaddr_in		in_addr;
	Input			input(argc, argv);

	if (argc == 3)	
	{
		input.parse(in_addr);
		//Server.run(argv);
	}
	else
		write(1,"test",4);
		//Print.usage();
	return (1);
}
