
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include <cstring>
#include <cerrno>

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
	struct hdata *hdata_h;

	hdata = (struct info*)malloc(sizeof(struct info));
	hdata->name = "PONG\n";
	hdata->value = "12\n";
	hdata_h = (struct hdata*)malloc(sizeof(struct hdata));
	hdata_h->name = "version";
	hdata_h->value = "1.2";
	buffer = (char *)malloc(sizeof(char) * 100);
	bzero(buffer, 100);
	len = recv(socket, buffer, 100, 0);
	if ((len < 0 && (errno != EWOULDBLOCK)))
		exit(1);
	(void)len;
	write(1, buffer, 100);
	if (strncmp(buffer, "JOIN #can", strlen("JOIN #can")) == 0)
	{
		int total = 0;
		std::string msg = ":ssergiu!ssergiu@127.0.0.1 JOIN :#can\n";
		int left = msg.length();
		int len;

		while (total < (int)msg.length())
		{
			len = send(socket, msg.c_str() + total, left, 0);
			total += len;
			left -= len;
		}

		total = 0;
		msg = ":127.0.0.1 332 ssergiu #can :something\n";
		left = msg.length();
		while (total < (int)msg.length())
		{
			len = send(socket, msg.c_str() + total, left, 0);
			total += len;
			left -= len;
		}

		total = 0;
		msg = ":127.0.0.1 353 ssergiu = #can :@ssergiu\n";
		left = msg.length();
		while (total < (int)msg.length())
		{
			len = send(socket, msg.c_str() + total, left, 0);
			total += len;
			left -= len;
		}

		total = 0;
		msg = ":127.0.0.1 366 ssergiu #can :End of NAMES list\n";
		left = msg.length();
		while (total < (int)msg.length())
		{
			len = send(socket, msg.c_str() + total, left, 0);
			total += len;
			left -= len;
		}
		//len = send(socket, "JOIN #can :ssergiu\n", strlen("JOIN #can\n"), 0);
		(void)len;
	}
	if (strncmp(buffer, "PING", strlen("PING")) == 0)
	{
		int len;

		len = send(socket, "PONG\n", strlen("PONG\n"), 0);
		(void)len;
	}
	if (strncmp(buffer, "CAP", strlen("CAP")) == 0)
	{
		int len;

		len = send(socket, ":127.0.0.1 001 ssergiu: Welcome to the server, ssergiu! \r\n", strlen(":127.0.0.1 001 ssergiu: Welcome to the server, ssergiu! \r\n"), 0);
		(void)len;
	}
	if (strncmp(buffer, "PRIVMSG #can", strlen("PRIVMSG #can")) == 0)
	{
		int len;

		len = send(socket, ":ssergiu PRIVMSG #can :test!\r\n", strlen(":ssergiu PRIVMSG #can :test!\r\n"), 0);
		(void)len;
	}
	return (len);
}

void Server::send_messages(int socket)
{
	int len;

	len = send(socket, ":127.0.0.1 001 ssergiu: Welcome to the server, ssergiu! \n", strlen(":127.0.0.1 001 ssergiu: Welcome to the server, ssergiu! \n"), 0);
	(void)len;
}
	
void Server::registerClient(int socket)
{
	Client *newClient = new Client(socket);	
	clients.push_back(newClient);
}
