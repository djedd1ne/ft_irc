
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>

class Client;

class Channel
{
	private:
		std::string name;
		std::vector <Client *> userList;
	public:
		Channel(std::string &, Client*);
		Channel(int &);
		Channel(const Channel&);
		Channel operator= (const Channel&);
		~Channel(void);
};

#endif
