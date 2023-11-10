
#include "../inc/Server.hpp"

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

void Server::read_messages(int socket)
{
	char *buffer;
	int len;

	buffer = (char *)malloc(sizeof(char) * 10);
	len = recv(socket, buffer, 10, 0);
	printf("buffer: %s\n", buffer);
	printf("len: %d\n", len);
	buffer[9] = 0;
	write(1, buffer, 10);
}
