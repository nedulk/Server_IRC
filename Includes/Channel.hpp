//
// Created by gangouil on 7/11/24.
//

#ifndef SERVER_IRC_CHANNEL_HPP
#define SERVER_IRC_CHANNEL_HPP

#include "irc.hpp"

class Channel
{
private:
	std::map<int, Client*>	_userList;
	std::map<int, Client*>	_operators;
	std::map<int, Client*>	_invited;
	std::string				_key;
	std::string				_name;
	std::string				_topic;
	int 					_userLimit;
	int 					_userCount;

	bool	_inviteOnly;
	bool	_topicRestrictions;
	bool	_isChannelKey;
	bool	_isUserLimit;

	void	isValidName(std::string name);

public:
	Channel(std::string& name);
	Channel(std::string& name, std::string key);

	int						getUserLimit() const;
	int 					getUserCount() const;
	bool					getInviteOnly() const;
	bool					getTopicRestr() const;
	bool					getIsChannelKey() const;
	bool					getIsUserLimit() const;
	std::string&			getKey();
	std::string&			getTopic();
	std::string&			getName();
	std::map<int, Client*>	getUserList();
	std::map<int, Client*>	getOperators();
	bool					isInvited(int fd) const;

	void	delUser(Client *user);
	void	addUser(Client *user);
	void	addOperator(Client *user);
	void	delOperator(Client *user);
	void	addInvite(Client *user);
	void	delInvite(int fd);
	void	setKey(std::string& key);
	void	setIsChannelKey(bool state);
	void	setTopic(std::string& topic);

	void	broadcastUserList(Client& client);
};

#endif //SERVER_IRC_CHANNEL_HPP
