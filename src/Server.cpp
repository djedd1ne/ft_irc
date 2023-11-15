
#include "../inc/Server.hpp"
#include <cstring>

// Constructors

Server::Server(void)
{
}

Server::Server(const Server &src)
{
	(void) src;
}

Server Server::operator= (const Server &src)
{
	(void)src;
	return (*this);
}

Server::~Server(void)
{
}

// Functions

int	Server::create_socket(void)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1) 
	{
        perror("Failed to create socket");
        return (-1);
    }
	return (socket_fd);
}

void Server::bind_socket(int socket, addrinfo **test)
{
    if (bind(socket, (*test)->ai_addr, (*test)->ai_addrlen))
	{
        perror("Failed to bind socket");
        close(socket);
        exit(1);
    }
}

void Server::start_listening(int socket)
{
    if (listen(socket, SOMAXCONN) == -1) 
	{
        perror("Failed to listen for connections");
        close(socket);
        exit(1);
    }
}

int Server::accept_conn(int socket, addrinfo **test)
{
	int clientSocket = accept(socket, (*test)->ai_addr, &(*test)->ai_addrlen);

	if (clientSocket == -1) 
	{
		perror("Failed to accept incoming connection");
		close(clientSocket);
		exit(1);
    }
	else
	{
		std::cout << "ACCEPTED :: "  << std::endl;
	}
	return (clientSocket);
}

int Server::read_messages(int socket)
{
	char *buffer;
	int len;
	struct info *hdata;

	hdata = (struct info*)malloc(sizeof(struct info));
	hdata->name = "version";
	hdata->value = "12\n";
	buffer = (char *)malloc(sizeof(char) * 100);
	bzero(buffer, 100);
	if ((recv(socket, buffer, 100, 0) < 0) and (errno != EWOULDBLOCK))
		exit(1);
	(void)len;
	write(1, buffer, 10);
	if (strncmp(buffer, "INFO version", 12) == 0)
	{
		printf("openng buffer\n");
		int len;

		len = send(socket, hdata, sizeof(hdata), 0);
		printf("sending reply len: %d\n", len);
		printf("sent buffer\n");
		(void)len;
	}
	if (strncmp(buffer, "PING", 4) == 0)
	{
		printf("sending reply\n");
		int len;

		len = send(socket, hdata->name.c_str(), hdata->name.length(), 0);
		(void)len;
	}
	return (1);
}

void Server::send_messages(int socket)
{
	int len;

	len = send(socket, ":127.0.0.1 001 ssergiu: Welcome to the server, ssergiu! \n", strlen(":127.0.0.1 001 ssergiu: Welcome to the server, ssergiu! \n"), 0);
	(void)len;
}
