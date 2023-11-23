
#include "../inc/Server.hpp"
#include "../inc/Input.hpp"
#include "../inc/Client.hpp"
#include <cstring>
#include <cerrno>
#include <arpa/inet.h>

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
	Client *newClient = new Client();	
	clients.push_back(newClient);
	int clientSocket = newClient->acceptConnection(this->_socket);

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
	if(clients[clientIndex]->isRegistered() == false)
	{
		if(cmd[0] == "CAP" && clients[clientIndex]->wasCapSent() == false)
			clients[clientIndex]->capSent();
		if (clients[clientIndex]->wasCapSent() == true)
		{
			if (cmd[0] == "PASS")
			{
				printf("cmd[1] is : -%s-\n", cmd[1].c_str());
				printf("password -%s-\n", password.c_str());
				if (cmd[1] == password+"\n")
				{
					printf("yes\n");
					send(socket, "password is correct", strlen("password is correct"), 0);
					clients[clientIndex]->passSent();
				}
			}
			if (clients[clientIndex]->wasPassSent() == true && cmd[0] == "NICK")
			{
				clients[clientIndex]->setNick(cmd[1]);
				clients[clientIndex]->nickSent();
			}
			if (clients[clientIndex]->wasPassSent() == true && cmd[0] == "USER")
			{
				clients[clientIndex]->setUsername(cmd[1]);
				clients[clientIndex]->userSent();
				send(socket, ":0.0.0.0 001 ssergiu: Welcome to the server, ssergiu! \r\n", strlen(":0.0.0.0 001 ssergiu: Welcome to the server, ssergiu! \r\n"), 0);
				clients[clientIndex]->registerUser();
			}
		}
				
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
		(void)socket;
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
	std::string msg;

	clientIndex = findClient(socket);
	msg = getMsg(socket);
	if (!msg.empty())
		command = parseMsg(msg);
	else
	{
		struct sockaddr_storage *client = clients[clientIndex]->getClientAddr();
        struct sockaddr_in *ptr = (struct sockaddr_in *)&client;
		std::cout << "DISCONNECT :: "  << inet_ntoa(ptr->sin_addr)<<std::endl;
		return (0);
	}
	//debugging purposes
	for (size_t i = 0; i < command.size(); i++)
		printf("COMMAND [%ld]: %s\n", i, command[i].c_str());
	// ---------------
	if (!command.empty())
	{
		handleCommand(command, socket, clientIndex);
		return(1);
	}
	return (0);
}

void Server::send_messages(int socket)
{
	int len;

	len = send(socket, ":0.0.0.0 001 ssergiu: Welcome to the server, ssergiu! \n", strlen(":0.0.0.0 001 ssergiu: Welcome to the server, ssergiu! \n"), 0);
	(void)len;
}

void Server::polling(void)
{
	int existingConns;
	int pollc;
	this->conn[0].fd = getSocket();
	this->conn[0].events = POLLIN;
	existingConns = 1;

	while(1)
	{
		pollc = poll(conn, existingConns, -1);
		(void)pollc;
		//iterate over conns
		for (int i = 0; i < existingConns; i++)
		{
			// if file descriptor is ready to read
			if (this->conn[i].revents & POLLIN)
			{
				//if server is ready to read, handle new conn
				if (this->conn[i].fd == getSocket())
				{
					this->conn[existingConns].fd = accept_conn();
					this->conn[existingConns].events = POLLIN;
					existingConns++;
					
					std::cout << "New connection accepted: "<<std::endl;
				}
				//if not listener then its just a regular client
				else
				{
					if (readMsg(conn[i].fd) == 0)
					{
						close(conn[i].fd);
						printf("Client Disconnected\n");
						conn[i] = conn[existingConns - 1];
						existingConns--;
					}
				}
			}
		}
	}
}
