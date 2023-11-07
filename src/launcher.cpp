
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
	sockaddr		*addr;
	Input			input(argc, argv);

	addr = (sockaddr*)malloc(sizeof(sockaddr));

	// first arg in socket creation;
	addr->sa_family = AF_INET;

	if (argc == 3)	
	{
		input.parse(in_addr);
		//Server.run(argv);
	}
	else
		write(1,"test",4);
		//Print.usage();
	printf("sa_family test: %d\n", addr->sa_family);
	return (1);
}
