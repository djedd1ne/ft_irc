
#include "../inc/Input.hpp"
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
	sockaddr_in		in_addr;
	sockaddr		*addr;
	addrinfo		*test;
	Input			input(argc, argv);
	std::string		ip("127.0.0.1");
	std::string		port("80");

	addr = (sockaddr*)malloc(sizeof(sockaddr));

	int status;

	status = getaddrinfo(ip.c_str(), port.c_str(), NULL, &test);
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
	printf("ai_flags : %d\n", test->ai_flags);
	printf("ai_family : %d\n", test->ai_family);
	printf("ai_socktype : %d\n", test->ai_socktype);
	printf("ai_protocol : %d\n", test->ai_protocol);
	printf("canoname : %s\n", test->ai_canonname);
	printf("socklen : %u\n", test->ai_addrlen);
	printf("sockaddr.1  : %d\n", test->ai_addr->sa_family);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[0]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[1]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[2]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[3]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[4]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[5]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[6]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[7]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[8]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[9]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[10]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[11]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[12]);
	printf("sockaddr.2  : %d\n", test->ai_addr->sa_data[13]);
	printf("sockaddr.2 len  : %lu\n", sizeof(test->ai_addr->sa_data));
	return (1);
}
