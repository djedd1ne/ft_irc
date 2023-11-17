
#include "../inc/Server.hpp"
#include "../inc/Input.hpp"
#include "../inc/Client.hpp"
#include <cstring>
#include <cerrno>

// Constructors

Server::Server(char **string)
{
	this->port = string[1];
	this->password = string[2];
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

void Server::setAddrInfo(void)
{
	getaddrinfo(MY_DOMAIN, this->port, NULL, &this->addr);
}

int	Server::getSocket(void)
{
	return (this->_socket);
}

void Server::create_socket(void)
{
    this->_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (this->_socket == -1) 
	{
        perror("Failed to create socket");
        exit(1);
    }
}

void Server::bind_socket(void)
{
    if (bind(this->_socket, this->addr->ai_addr, this->addr->ai_addrlen))
	{
        perror("Failed to bind socket");
        close(this->_socket);
        exit(1);
    }
}

void Server::start_listening(void)
{
    if (listen(this->_socket, SOMAXCONN) == -1) 
	{
        perror("Failed to listen for connections");
        close(this->_socket);
        exit(1);
    }
}

int Server::accept_conn(void)
{
	int clientSocket = accept(this->_socket, this->addr->ai_addr, &this->addr->ai_addrlen);

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

std::vector<std::string> Server::parseMsg(std::string msg)
{
	std::vector<std::string> res;	
	size_t pos = 0;
	
	while (pos < msg.size())
	{
		pos = msg.find(" ");
		res.push_back(msg.substr(0, pos));
		msg.erase(0, pos + 1);
	}
	return res;
}

std::string Server::getMsg(int socket)
{
	char *buffer;
	std::string msg("");

	buffer = (char *)malloc(sizeof(char) * 256);
	bzero(buffer, 256);
	recv(socket, buffer, 255, 0);
	msg = buffer;
	return (msg);
}

void Server::handleCommand(std::vector<std::string> cmd, int socket, int clientIndex)
{
	if(clients[clientIndex]->isRegistered() == false && cmd[0] == "CAP")
	{
		caplsCmd(cmd, socket);
		clients[clientIndex]->registerUser();
	}
	else if (clients[clientIndex]->isRegistered())
	{
		if(cmd[0] == "JOIN")
			joinCmd(cmd, socket);
		else if(cmd[0] == "PING")
			pingCmd(cmd, socket);
		else if(cmd[0] == "PRIVMSG")
			privMsgCmd(cmd, socket);
	}
	else
		send(socket, "Please register first!", strlen("Please register first!"), 0);
		
}

void Server::privMsgCmd(std::vector<std::string> cmd, int socket)
{
	if (cmd[0] == "PRIVMSG")
	{
		int len;

		len = send(socket, ":ssergiu PRIVMSG #can :test!\r\n", strlen(":ssergiu PRIVMSG #can :test!\r\n"), 0);
		(void)len;
	}
}

void Server::pingCmd(std::vector<std::string> cmd, int socket)
{
	if (cmd[0] == "PING")
	{
		int len;

		len = send(socket, "PONG\n", strlen("PONG\n"), 0);
		(void)len;
	}
}

void Server::caplsCmd(std::vector<std::string> cmd, int socket)
{
	if (cmd[0] == "CAP")
	{
		int len;
		(void)len;
		len = send(socket, ":0.0.0.0 001 ssergiu: Welcome to the server, ssergiu! \r\n", strlen(":0.0.0.0 001 ssergiu: Welcome to the server, ssergiu! \r\n"), 0);
	}
}

void Server::joinCmd(std::vector<std::string> cmd, int socket)
{
	if (cmd[0] == "JOIN")
	{
		int total = 0;
		std::string msg = ":ssergiu!ssergiu@0.0.0.0 JOIN :" + cmd[1] + "\r\n";
		int left = msg.length();
		int len;

		while (total < (int)msg.length())
		{
			len = send(socket, msg.c_str() + total, left, 0);
			total += len;
			left -= len;
		}

		total = 0;
		msg = ":0.0.0.0 332 ssergiu" + cmd[1] + ":something\r\n";
		left = msg.length();
		while (total < (int)msg.length())
		{
			len = send(socket, msg.c_str() + total, left, 0);
			total += len;
			left -= len;
		}

		total = 0;
		msg = ":0.0.0.0 353 ssergiu = " + cmd[1] + ":@ssergiu, djmekki, doreshev, azer\r\n";
		left = msg.length();
		while (total < (int)msg.length())
		{
			len = send(socket, msg.c_str() + total, left, 0);
			total += len;
			left -= len;
		}

		total = 0;
		msg = ":0.0.0.0 366 ssergiu " + cmd[1] + ":End of NAMES list\r\n";
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
}
size_t	Server::findClient(int socket)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i]->getSocket() == socket)
			return (i);
	}
	return (0);
}

int Server::readMsg(int socket)
{
	int clientIndex;

	clientIndex = findClient(socket);
	command = parseMsg(getMsg(socket));
	//debugging purposes
	for (size_t i = 0; i < command.size(); i++)
		printf("COMMAND [%ld]: %s\n", i, command[i].c_str());
	// ---------------
	if (!command.empty())
		handleCommand(command, socket, clientIndex);
	return (1);
}

void Server::send_messages(int socket)
{
	int len;

	len = send(socket, ":0.0.0.0 001 ssergiu: Welcome to the server, ssergiu! \n", strlen(":0.0.0.0 001 ssergiu: Welcome to the server, ssergiu! \n"), 0);
	(void)len;
}
	
void Server::registerClient(int socket)
{
	Client *newClient = new Client(socket);	
	clients.push_back(newClient);
}
