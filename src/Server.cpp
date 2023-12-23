
#include "../inc/Server.hpp"
#include "../inc/Input.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
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
    if (listen(this->_socket, 10) == -1) 
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
	for (size_t k = 0;k < res.size();k++)
		std::cout << res[k] << "*";
	std::cout << std::endl;
	return res;
}
// std::vector<std::string> Server::parseMsg(std::string str)
// {
// 	char sep = ' ' ;
// 	std::vector<std::string> row;
// 	std::string word;
// 	std::stringstream s(str);
// 	while (std::getline(s, word, sep)) {
// 		if (word.size())
// 			row.push_back(word);
// 	}
// 	for (size_t k = 0;k < row.size();k++)
// 		std::cout << row[k] << "*";
// return row;
//	}

std::string Server::getMsg(int socket)
{
	char *buffer;
	std::string msg("");

	buffer = (char *)malloc(sizeof(char) * 256);
	bzero(buffer, 256);
	recv(socket, buffer, 255, 0);
	msg = buffer;
	std::cout << msg << std::endl;
	return (msg);
}

std::string Server::extract_Topic_ssg( int ind) {
    std::string mssg = "";
    for (int indd = ind; indd < static_cast<int>(command.size()); indd++) {
        mssg += command[indd];
        mssg += " ";
    }
    return mssg;
}

void Server::ft_topic(int clientIndex, int socket)
{
	//int ind_chan = getChanIndex(command[ 1]);
	if (command.size() == 1)
	    sendUser("461 " + clients[clientIndex]->getNick() + "TOPIC :Not enough parameters", clients[clientIndex]->getSocket());
	else if ((command.size() > 2))
	{
	    std::string topic = extract_Topic_ssg(2);
	    channelList[getChanIndex(command[1])]->setTopic(topic);
	    sendUser("332 " + clients[clientIndex]->getNick() + " " + command[1] + " " + topic, socket);
	    for (size_t k = 0; k < clients.size(); k++)
	    {
	        for (std::size_t l = 0; l < channelList[getChanIndex(command[1])]->userList.size(); l++)
	        {
	            if (clients[k]->getSocket() == channelList[getChanIndex(command[1])]->userList[l]->getSocket())
	                sendUser(":" + clients[clientIndex]->getNick() + "!~" + clients[clientIndex]->getUsername() + "@localhost  TOPIC " + channelList[getChanIndex(command[ 1])]->getName() + " " + topic, clients[k]->getSocket());
	        }
	    }
	}
	// else if ((channelList[ind_chan]->mode_t == true && srch_is_operator(clients[clientIndex].nickname, clients[clientIndex].socket, channelList, ind_chan) == -1))
	//     sendUser("482 " + clients[clientIndex].nickname + " " +   channelList[ind_chan].name +  " :You're not a channel operator",clients[clientIndex].socket);
	else if (command.size() >= 2 && getChanIndex(command[1]) == -1)
	    sendUser("403 " + clients[clientIndex]->getNick() + " " + command[1], clients[clientIndex]->getSocket()); 
}


void Server::ft_invite(int clientIndex)
{
     int ind_chan = getChanIndex(command[2]);
     if (ind_chan == -1)
         sendUser("403 " + clients[clientIndex]->getNick() + " " + command[2], clients[clientIndex]->getSocket());
     else if (searchByNickName(command[1],clients.size()) == -1)
          sendUser("401 " + clients[clientIndex]->getNick() + " " +  command[1] + " :No such nick/channel",clients[clientIndex]->getSocket());
    //  else if (srch_is_operator(clients[clientIndex]->getNick(), clients[clientIndex]->getSocket(), clientIndex, ind_chan) == -1)
    //      sendUser("482 " + clients[clientIndex]->getNick() + " " +  clientIndex[ind_chan]->getName() +  " :You're not a channel operator",clients[clientIndex]->getSocket());
     else
     {
         sendUser(":[" + clients[clientIndex]->getNick() + "] INVITE " + command[1] + " :" + command[2], clients[clientIndex]->getSocket());
         sendUser("341 " + clients[clientIndex]->getNick() + " " + command[1] + " " + command[2], clients[clientIndex]->getSocket());
         sendUser(":" + clients[clientIndex]->getNick()  + " INVITE " + command[1] + " " + command[2], clients[searchByNickName(command[1], clients.size())]->getSocket());
         channelList[ind_chan]->invited.push_back(command[1]);
     }
}
void Server::handleCommand(std::vector<std::string> cmd, int socket, int clientIndex)
{
	if(!clients[clientIndex]->isRegistered())
		capLsCmd(cmd, socket, clientIndex);			
	else if (clients[clientIndex]->isRegistered())
	{
		if(cmd[0] == "JOIN")
			joinCmd(cmd, socket, clientIndex);
		else if(cmd[0] == "PING")
			pingCmd(cmd, socket);
		else if(cmd[0] == "PRIVMSG")
			privMsgCmd(cmd, clientIndex);
		else if (cmd[0] == "TOPIC")
			ft_topic(clientIndex,socket);
		else if (cmd[0] == "INVITE")
			ft_invite(clientIndex);
		else if (cmd[0] ==  "MODE")
			ft_mode(clientIndex);
	}
	else
		send(socket, "Please register first!", strlen("Please register first!"), 0);
		
}

int Server::srch_clnt_chan(const int clientSocket,int ind)
{
	for (size_t i = 0; i < channelList[ind]->userList.size(); i++)
	{
		if (channelList[ind]->userList[i]->getSocket() == clientSocket)
		return (i);
	}
	return (-1);
}

int Server::srch_is_operator(std::string nickname,const int clientSocket,int ind)
{
	if (srch_clnt_chan(clientSocket,ind) != -1)
	{
		for (size_t i = 0; i < channelList[ind]->admins_users.size(); i++)
		{
			if (nickname == channelList[ind]->admins_users[i]->getNick())
			return (0);
		}
	}
	return (-1);
}
std::string Server::get_modes(int ind)
{
	std::string mode = " +";
		if (channelList[ind]->mode_i == true)
			mode += "i";
		if (channelList[ind]->mode_t == true)
			mode += "t";
		if (channelList[ind]->mode_l == true)
			mode += "l";
		if (channelList[ind]->mode_k == true)
			mode += "k";
	return (mode);
}

void Server::ft_mode(int clientIndex)
{
	int ind_chan = getChanIndex(command[1]);
    if (command.size() == 2 && ind_chan != -1)
    {
		for (size_t i = 0 ; i < command.size() ; i++)
			std::cout << command[i] << "-";
		std::cout << std::endl;
        std::string mode = get_modes(ind_chan);
        if (channelList[ind_chan]->mode_l == true)
            mode += " " + channelList[ind_chan]->limit;
        if (channelList[ind_chan]->mode_k == true)
            mode += " " + channelList[ind_chan]->password;
        sendUser("324 " + clients[clientIndex]->getNick() + " " + command[1] + mode, clients[clientIndex]->getSocket());
    }
    else if (command[1] == clients[clientIndex]->getNick());
    else if (ind_chan == -1)
        sendUser("403 " + clients[clientIndex]->getNick() + " " + command[1], clients[clientIndex]->getSocket());
    else if (srch_is_operator(clients[clientIndex]->getNick(), clients[clientIndex]->getSocket(),ind_chan) == -1)
        sendUser("482 " + clients[clientIndex]->getNick() + " " +  command[1] +  " :You're not a channel operator",clients[clientIndex]->getSocket());
	else if (command.size() > 2 && ind_chan != -1 && srch_is_operator(clients[clientIndex]->getNick(), clients[clientIndex]->getSocket(), ind_chan) != -1)
    {
		
		if ((command[2][0] == '+' || command[2][0] == '-') && command[2].size() <= 2)
        {
			std::string tmp = "";
            tmp += command[2][0];

            for (size_t p = 1; p < command[2].size(); p++)
            {
				if (command[2][p] != 'i' && command[2][p] != 't' && command[2][p] != 'k' && command[2][p] != 'l' && command[2][p] != 'o')
            	   sendUser("472 " + clients[clientIndex]->getNick() + " " + command[2][p] + " :Unknown mode", clients[clientIndex]->getSocket());
				if ((command[2][0] == '+' || command[2][0] == '-') && command[2][p] == 'i')
        		   {
        		       if (command[2][0] == '-')
        		           channelList[ind_chan]->mode_i = false;
        		       else
        		           channelList[ind_chan]->mode_i = true;
        		       tmp += command[2][p];
        		   }
				}
		if (tmp != "+" && tmp != "-")
        for (size_t k = 0; k < channelList[ind_chan]->userList.size(); k++)
            sendUser(":" + clients[clientIndex]->getNick() + "!~" +clients[clientIndex]->getUsername() + "@localhost MODE " + command[1] + " " + tmp , channelList[ind_chan]->userList[k]->getSocket());
		}
		else
        sendUser("472 " + clients[clientIndex]->getNick()+ " " + command[2] + " :Unknown mode", clients[clientIndex]->getSocket());
	}
}

void Server::sendUser(const std::string& msg, int clientSocket)
{
	std::string msgError = msg + "\r\n";
	send(clientSocket, msgError.c_str(), msgError.length(), 0);
	return ;
}

void Server::privMsgCmd(std::vector<std::string> cmd, int clientIndex)
{
	
	std::string message;
    for (size_t i = 2; i < cmd.size(); ++i) {
        message += cmd[i];
        if (i < cmd.size() - 1) {
            message += ' ';
        }
    }
    message = message.substr(1);
	int ind = searchByNickName(cmd[1],clients.size());


 	if (cmd[1][0] == '#')
    {
        int ind2 = getChanIndex(cmd[1]);
        if (ind2 == -1)
            sendUser("403 " + clients[clientIndex]->getNick() + " " + cmd[2], clients[clientIndex]->getSocket());
        // else if(srch_clnt_chan(clients[[clientIndex]->getSocket(),channelList,ind2) == -1)
        //     sendUser("404 " + clients[clientIndex]->getNick() +  " " + cmd[1] + " :Cannot send to channel",clients[clientIndex]->getSocket());
        else
        {
        for (std::size_t k = 0; k < channelList[ind2]->userList.size(); k++)
        {
            if (channelList[ind2]->userList[k]->getSocket() != clients[clientIndex]->getSocket())
                sendUser(":" + clients[clientIndex]->getNick() + " PRIVMSG " + channelList[ind2]->getName() + " " + message, channelList[ind2]->userList[k]->getSocket());
        }
        }
    }

	if (ind != -1)
	{
		std::string msg = ":" + clients[clientIndex]->getNick() +  "!~" + clients[clientIndex]->getUsername()+ "@" + clients[clientIndex]->getUsername() + " PRIVMSG " + cmd[1] + " " + message + "\r\n";
		send(clients[ind]->getSocket(), msg.c_str(), msg.size(), 0);	
	}
}

void Server::pingCmd(std::vector<std::string> cmd, int socket)
{
	if (cmd[0] == "PING")
		send(socket, "PONG\n", strlen("PONG\n"), 0);
}

int Server::searchByNickName(const std::string& target, int numClients)
{
	std::string target2 = target;
	for (int i = 0; i < numClients; i++)
	{
		if (strcmp(clients[i]->getNick().c_str(), target2.c_str()) == 0)
			return i;
	}
	return -1;
}

void Server::capLsCmd(std::vector<std::string> cmd, int socket, int clientIndex)
{
	std::string msg;

	for(size_t j = 0; j < cmd.size() ;j++)
	{
		if(cmd[j] == "CAP" && clients[clientIndex]->wasCapSent() == false)
		{
			msg = ":SERVER NOTICE client: => Please send USER, NICK and PASS to complete the "
				"registration! \r\n : => /quote NICK nick, /quote USER user and /quote PASS **** <==\r\n";
			//msg = "CAP*LS\r\n";
			send(socket, msg.c_str(), msg.size(), 0);
			clients[clientIndex]->capSent();
		}
		else if (clients[clientIndex]->wasCapSent() == true)
		{
			if (cmd[j] == "NICK")
			{
				clients[clientIndex]->setNick(cmd[j + 1]);
				clients[clientIndex]->nickSent();
				std::cout<<"nick\n"<<std::endl;
			}
			if (cmd[j] == "USER")
			{
				clients[clientIndex]->setUsername(cmd[j + 1]);
				clients[clientIndex]->userSent();
				std::cout<<"user\n"<<std::endl;
			}
			if (cmd[j] == "PASS")
			{
				if (cmd[j + 1] == password)
				{
					clients[clientIndex]->passSent();
					clients[clientIndex]->registerUser();

					msg = ":127.0.0.1 001 " + clients[clientIndex]->getNick() + " : Welcome to the server "+
					clients[clientIndex]->getNick() + "@" + clients[clientIndex]->getIp() + "!\r\n";
					send(socket,"CAP * LS\r\n",strlen("CAP * LS\r\n"),0);
					send(socket, msg.c_str(), msg.size(), 0);
					msg = ":tmp_nick NICK " + clients[clientIndex]->getNick();
					send(socket,msg.c_str(),msg.size(),0);
				}
			}
		}
	}
}

int Server::getChanIndex(std::string &chan)
{
	for (size_t i = 0; i < channelList.size(); i++)
		if (channelList[i]->getName() == chan)
			return (i);
	return (-1);
}

Channel* Server::createChannel(std::string &chan)
{
	Channel *newChannel = new Channel(chan);	
	channelList.push_back(newChannel);
	return (newChannel);
}

/////////////////////////////////////////// Also this /////////////////////////////////////////

int Server::srch_admin_users(std::string nickname, std::vector <Channel *> userchannelList, int ind)
{
	for (size_t i = 0; i < userchannelList[ind]->admins_users.size();i++)
	{
		if (nickname == userchannelList[ind]->admins_users[i]->getNick())
			return i;
	}
	return (-1);
}

int Server::check_is_invited(std::string nickname, int ind_chan)
{
	for(size_t i = 0; i < channelList[ind_chan]->invited.size();i++)
	{
		if (nickname == channelList[ind_chan]->invited[i])
			return (0);
	}
	return (-1);
}

void Server::execJoin(std::vector<std::string> cmd, int ind_chan ,int socket, int clientIndex)
{
		//channelList[ind_chan]->userList.push_back(clients[clientIndex]);
		if (channelList[ind_chan]->mode_i == true && check_is_invited(clients[clientIndex]->getNick(),ind_chan))
        	sendUser("473 " + clients[clientIndex]->getNick() + " " + cmd[1] + " :Cannot join channel (+i)", clients[clientIndex]->getSocket());
		else
		{
			std::string msg = "324 " + clients[clientIndex]->getNick() + " " + channelList[ind_chan]->getName() + " +\r\n";
			send(socket, msg.c_str(), msg.size(), 0);
			if (channelList[ind_chan]->getTopic() != "")
			{
				msg = "332 " + clients[clientIndex]->getNick() + " " + cmd[1] + " " + channelList[ind_chan]->getTopic() + "\r\n";
				send(socket, msg.c_str(), msg.size(), 0);
			}
			std::string test = " :";
        	for (size_t k = 0; k < clients.size(); k++)
        	{
        	    for (std::size_t l = 0; l < channelList[ind_chan]->userList.size(); l++)
        	    {
        	        if (clients[k]->getSocket() == channelList[ind_chan]->userList[l]->getSocket())
        	        {
        	            if (srch_admin_users(clients[k]->getNick(), channelList,ind_chan) != -1)
        	                test+= "@";
        	            test += clients[k]->getNick() + " ";
        	        }
        	    }
        	}
	    	msg = "353 " + clients[clientIndex]->getNick() + " = " + cmd[1] + test + "\r\n";
			std::cout<<"usrr in chan: ";
			send(socket, msg.c_str(), msg.size(), 0);

			if (channelList[ind_chan]->userList.size() > 1)
        	{
        	    for (std::size_t l = 0; l < channelList[ind_chan]->userList.size(); l++)
        	    {
					msg = ":" + clients[clientIndex]->getNick() + "!~" +clients[clientIndex]->getUsername() + "@localhost JOIN " + cmd[1] + "\r\n" ;
        	        send(channelList[ind_chan]->userList[l]->getSocket(), msg.c_str(), msg.size(), 0);
        	    }
        	}
			msg = "366 " + clients[clientIndex]->getNick() + " " + cmd[1] + " :End of /NAMES list\r\n" ;
        	send( socket,msg.c_str(),msg.size(),0);
		}
}

void Server::joinCmd(std::vector<std::string> cmd, int socket, int clientIndex)
{
	if (cmd[0] == "JOIN")
	{
		Channel *chan;
		int chanIndex = getChanIndex(cmd[1]);
		std::cout<<"----------------------chan index: "<< chanIndex <<std::endl;
		if (chanIndex == -1)
		{
			chan = createChannel(cmd[1]);
			channelList[getChanIndex(cmd[1])]->admins_users.push_back(clients[clientIndex]);
		}
		channelList[getChanIndex(cmd[1])]->addUser(clients[clientIndex]);
		execJoin(cmd,getChanIndex(cmd[1]),socket,clientIndex);

	}
debug_stats();
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
	if (!command.empty())
	{ 
		handleCommand(command, socket, clientIndex);                ////////////////////////////////////////////////////////////--1
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

void Server::debug_stats(void)
{
	if (DEBUG & 1)
	{
		std::cout<<"DEBUG MODE ON"<<std::endl;
		if (!command.empty())
		{
			for (size_t i = 0; i < command.size(); i++)
			{
				std::cout<<"CMD["<<i;
				std::cout<<"]: "<<command[i]<<std::endl;
			}
		}
		if (!clients.empty())
		{
			for (size_t i = 0; i < clients.size(); i++)
			{
				std::cout<<"client["<<i;
				std::cout<<"]: "<<clients[i]->getNick()<<std::endl;
			}
		}
		if (!channelList.empty())
		{
			for (size_t i = 0; i < channelList.size(); i++)
			{
				std::cout<<"chan["<<i;
				std::cout<<"]: "<<channelList[i]->getName()<<std::endl;
					std::cout<<"usr in chan: ";
				for (size_t j = 0; j < channelList[i]->getUserList().size(); j++)
					std::cout<<channelList[0]->getUserList()[j]->getNick() << " ";
				std::cout << std::endl;
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
