
#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdlib.h>
#include <netinet/in.h>

class Server
{
	private:
	public:
		Server(int argc, char **argv);
		Server(const Server&);
		Server operator= (const Server&);
		~Server(void);

};

#endif
