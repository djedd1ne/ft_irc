
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	private:
		int socket;
		std::string _nick;
		//array of channels
		std::string _username;
		bool op;
		bool registered;
		bool _capSent;
		bool _passSent;
		bool _usrSent;
		bool _nickSent;
		Client(void);
	public:
		Client(int &);
		Client(const Client&);
		Client operator= (const Client&);
		~Client(void);
		void setSocket(int &sock);
		int getSocket(void);
		bool isRegistered();
		void registerUser();
		bool isOp();
		void capSent();
		bool wasCapSent();
		void setNick(std::string );
		void setUsername(std::string );
		void nickSent();
		void passSent();
		void userSent();
		bool wasNickSent();
		bool wasPassSent();
		bool wasUSerSent();
};

#endif
