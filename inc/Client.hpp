
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	private:
		int socket;
		std::string nick;
		//array of channels
		std::string username;
		bool op;
	public:
		Client(void);
		Client(int &);
		Client(const Client&);
		Client operator= (const Client&);
		~Client(void);
		void setSocket(int &sock);
		int getSocket(void);
};

#endif
