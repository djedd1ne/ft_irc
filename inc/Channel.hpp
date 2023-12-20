
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>

class Client;

class Channel
{
	private:
		std::string name;
		std::string topic;
		std::vector <Client *> userList;
	public:
		Channel(std::string &); 
		Channel(int &);
		Channel(const Channel&);
		Channel operator= (const Channel&);
		~Channel(void);
		std::string getName(void);
		std::string getTopic(void);
		void setTopic(std::string &);
		std::vector <Client *> getUserList(void);
		void addUser(Client *);
};

#endif
