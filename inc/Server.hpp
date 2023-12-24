
#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <poll.h>

#include <cctype>
#include <algorithm>
#include <sstream>


#define MY_DOMAIN "0.0.0.0"
#define CONNECTIONS 1000
#define DEBUG 1

class Client;
class Channel;
struct pollfd;

class Server
{
	private:
		struct pollfd conn[CONNECTIONS];
		std::string port;
		std::string password;
		int _socket;
		addrinfo *addr;
		std::vector <Channel *> channelList;
		std::vector <Client *> clients;
		std::vector <std::string> command;
		Server(void);
	public:
		Server(char **string);
		Server(const Server&);
		Server operator= (const Server&);
		~Server(void);
		void setAddrInfo(void);
		int	getSocket(void);
		std::string	getPort(void);
		void create_socket(void);	
		void bind_socket(void);
		void start_listening(void);
		size_t	findClient(int socket);
		int accept_conn(void);
		int readMsg(int );
		void polling(void);
		Channel* createChannel(std::string &chan);
		void handleCommand(std::vector<std::string>, int, int);
		void privMsgCmd(std::vector<std::string> , int clientIndex);
		void pingCmd(std::vector<std::string> , int);
		void capLsCmd(std::vector<std::string> , int, int);
		void joinCmd(std::vector<std::string> , int, int);
		void execJoin(std::vector<std::string> cmd,int ind_chan, int socket, int clientIndex);
		std::vector<std::string> parseMsg(std::string );
		std::string getMsg(int socket);
		void run(void);
		void debug_stats(void);
		int getChanIndex(std::string &);
		////////////////////////////////////// -- Adding this --  ////////////////////////////////////
		void remove_spaces(std::string& str);
		int srch_admin_users(std::string nickname,std::vector <Channel *>,int ind);
		int searchByNickName(const std::string& target, int numClients);
		void sendUser(const std::string& msg, int clientSocket);
		void ft_topic(int clientIndex, int socket);
		std::string extract_Topic_ssg(int ind);
		void ft_invite(int clinetIndex);
		void ft_kick(int clientIndex);
		void ft_part(int clientIndex);
		std::string get_modes(int ind);
		void ft_mode(int clientIndex);
		int	srch_is_operator(std::string nickname,const int clientSocket,int ind);
		int srch_clnt_chan(const int clientSocket,int ind);
		int check_is_invited(std::string nickname, int ind_chan);


};

#endif 
