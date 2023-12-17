
#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"

// Constructors

Channel::Channel(std::string &channelName, Client *clientName)
{
	this->name = channelName;
	this->userList.push_back(clientName);
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
