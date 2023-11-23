
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
	getaddrinfo(MY_DOMAIN, this->port.c_str(), NULL, &this->addr);
}

int	Server::getSocket(void)
{
	return (this->_socket);
}

std::string	Server::getPort(void)
{
	return (this->port);
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
    std::cout << "Server is listening on port " << this->getPort() << std::endl;
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
	if (msg.find("\r\n"))
		msg.replace(msg.find("\r\n"), 2, " ");
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
	if(!clients[clientIndex]->isRegistered())
		capLsCmd(cmd, socket, clientIndex);			
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

void Server::capLsCmd(std::vector<std::string> cmd, int socket, int clientIndex)
{
	std::string msg;

	if(cmd[0] == "CAP" && clients[clientIndex]->wasCapSent() == false)
	{
		msg = ":0.0.0.0 NOTICE client: Please send USER, NICK and PASS to complete the "
			"registration! \n ==> /quote NICK nick, /quote USER user and /quote PASS **** <==\r\n";
		send(socket, msg.c_str(), msg.size(), 0);
		clients[clientIndex]->capSent();
		if (cmd[0] == "NICK")
		{
			clients[clientIndex]->setNick(cmd[1]);
			clients[clientIndex]->nickSent();
			std::cout<<"nick\n"<<std::endl;
		}
		if (cmd[0] == "USER")
		{
			clients[clientIndex]->setUsername(cmd[1]);
			clients[clientIndex]->userSent();
			std::cout<<"user\n"<<std::endl;
		}
	}
	if (clients[clientIndex]->wasCapSent() == true)
	{
		if (cmd[0] == "PASS")
		{
			if (cmd[1] == password)
			{
				clients[clientIndex]->passSent();
				clients[clientIndex]->registerUser();
				msg = ":0.0.0.0 001 " + clients[clientIndex]->getNick() + ": Welcome to the server " +
				clients[clientIndex]->getNick() + "\r\n";
				send(socket, msg.c_str(), msg.size(), 0);
			}
		}
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

void Server::polling(void)
{
	int existingConns;
	int pollc;
	this->conn[0].fd = getSocket();
	this->conn[0].events = POLLIN;
	existingConns = 1;

	while(1)
	{
	//	std::cout<<"Server status: "<<std::endl;
	//	for (int i = 0; i < existingConns; i++)
	//		std::cout<<"poll ["<<i<<"]: "<<this->conn[i].fd<<std::endl;
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
				}
				//if not listener then its just a regular client
				else
				{
					if (readMsg(conn[i].fd) == 0)
					{
						close(conn[i].fd);
						conn[i] = conn[existingConns - 1];
						existingConns--;
					}
				}
			}
		}
	}
}

void Server::run(void)
{
	setAddrInfo();
	create_socket();
	bind_socket();
	start_listening();
	polling();
}
