
#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client
{
	private:
		int socket;
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
