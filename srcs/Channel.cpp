//
// Created by gangouil on 7/11/24.
//

#include "Channel.hpp"

void Channel::isValidName(std::string name)
{
	if (name.size() > 200)
		throw (std::runtime_error("Channel name too long"));
	if (name[0] != '&' && name[0] != '#')
		throw (std::runtime_error("Channel prefix invalid"));
	if (name.find_first_of(" ,") != std::string::npos || name.find(7) != std::string::npos)
		throw (std::runtime_error("Channel name contains space, comma or ^G"));
}

void Channel::broadcastUserList(Client &client)
{
	std::string	msg = ":ircserv ";
	std::string	nicknames;

	for (std::map<int, Client*>::iterator it = _userList.begin(); it != _userList.end(); ++it)
	{
		if (_operators.count(it->second->GetFd()) != 0)
			nicknames += "@";
		nicknames += it->second->GetNick() + " ";
	}
	msg += RPL_NAMREPLY(client.GetNick(), _name, nicknames) + "\r\n";
	msg += ":ircserv " + RPL_ENDOFNAMES(client.GetNick(), _name) + "\r\n";
	send(client.GetFd(), msg.c_str(), msg.size(), 0);
}

Channel::Channel(std::string& name)
{
	isValidName(name);
	_name = name;
	_userLimit = -1;
	_userCount = 0;
	_inviteOnly = false;
	_topicRestrictions = false;
	_isChannelKey = false;
	_isUserLimit = false;
}

Channel::Channel(std::string& name, std::string key)
{
	isValidName(name);
	_name = name;
	_key = key;
	_userLimit = -1;
	_userCount = 0;
	_inviteOnly = false;
	_topicRestrictions = false;
	_isChannelKey = true;
	_isUserLimit = false;
}

std::map<int, Client*> Channel::getUserList()
{
	return (_userList);
}

int Channel::getUserLimit() const {
	return (_userLimit);
}

int Channel::getUserCount() const {
	return (_userCount);
}

bool Channel::getInviteOnly() const {
	return (_inviteOnly);
}

bool Channel::getTopicRestr() const {
	return (_topicRestrictions);
}

bool Channel::getIsChannelKey() const {
	return (_isChannelKey);
}

bool Channel::getIsUserLimit() const {
	return (_isUserLimit);
}

std::string &Channel::getKey() {
	return (_key);
}

std::string & Channel::getTopic()
{
	return (_topic);
}

std::string & Channel::getName()
{
	return (_name);
}

void Channel::addUser(Client *user)
{
	_userList[user->GetFd()] = user;
	_userCount++;
}

void Channel::delUser(Client *user)
{
	_userList.erase(user->GetFd());
	_userCount--;
}


void Channel::setKey(std::string &key)
{
	_key = key;
}

void Channel::addOperator(Client *user)
{
	_operators[user->GetFd()] = user;
}

void Channel::delOperator(Client *user)
{
	try
	{
		_operators.erase(user->GetFd());
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Channel::addInvite(Client *user)
{
	_invited[user->GetFd()] = user;
}

void Channel::delInvite(int fd)
{
	try
	{
		_invited.erase(fd);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

bool Channel::isInvited(int fd) const
{
	return (_invited.count(fd) != 0);
}

std::map<int, Client*> Channel::getOperators()
{
	return (_operators);
}

void Channel::setIsChannelKey(bool state)
{
	_isChannelKey = state;
}

void Channel::setTopic(std::string &topic)
{
	_topic = topic;
}
