


#ifndef INPUT_HPP
#define INPUT_HPP

#include <stdlib.h>
#include <netinet/in.h>

class Input
{
	private:
		char	*password;
		int		port;
	public:
		Input(int argc, char **argv);
		Input(const Input&);
		Input operator= (const Input&);
		~Input(void);
		void parse(sockaddr_in &);

};

#endif
