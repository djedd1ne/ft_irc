
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

class Client
{
	private:
		int _socket;
		std::string _nick;
		//array of channels
		struct sockaddr_storage *clientAddr;
		socklen_t clientAddrLen;
		std::string _username;
		std::string _ip;
		bool op;
		bool registered;
		bool _capSent;
		bool _passSent;
		bool _usrSent;
		bool _nickSent;
	public:
		Client(void);
		Client(const Client&);
		Client operator= (const Client&);
		~Client(void);
		void setSocket(int &sock);
		int getSocket(void);
		struct sockaddr_storage *getClientAddr(void);
		int acceptConnection(int &);
		bool isRegistered();
		void registerUser();
		bool isOp();
		void capSent();
		bool wasCapSent();
		void setNick(std::string );
		void setUsername(std::string );
		std::string getNick(void);
		std::string getUsername(void);
		void nickSent();
		void passSent();
		void userSent();
		bool wasNickSent();
		bool wasPassSent();
		bool wasUSerSent();
};

#endif
