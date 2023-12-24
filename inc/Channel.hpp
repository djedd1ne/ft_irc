
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
	public:
		std::vector <Client *> userList;
		std::vector<Client *> admins_users;
		std::vector <std::string> invited;
		std::string limit;
		std::string password;
		bool mode_i;
   		bool mode_t;
    	bool mode_l;
    	bool mode_k;
		Channel(std::string &); 
		Channel(int &);
		Channel(const Channel&);
		Channel operator= (const Channel&);
		~Channel(void);
		std::string getName(void);
		std::string getTopic(void);
		void setTopic(std::string &);
		std::vector <Client *> getUserList(void);
		void removeUser(std::string name);
		void addUser(Client *);
};

#endif
