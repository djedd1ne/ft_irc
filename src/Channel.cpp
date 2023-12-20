
#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"

// Constructors

Channel::Channel(std::string &channelName) 
{
	this->name = channelName;
}

Channel::Channel(const Channel &src)
{
	(void) src;
}

Channel Channel::operator= (const Channel &src)
{
	(void)src;
	return (*this);
}

Channel::~Channel(void)
{
}

// Functions

std::vector <Client *> Channel::getUserList(void)
{
	return (this->userList);
}

std::string Channel::getName(void)
{
	return (this->name);
}

void Channel::addUser(Client *user)
{
	userList.push_back(user);
}

std::string Channel::getTopic(void)
{
	return (this->topic);
}

void Channel::setTopic(std::string &newTopic)
{
	this->topic = newTopic;
}
