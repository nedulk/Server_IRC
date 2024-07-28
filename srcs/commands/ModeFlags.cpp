//
// Created by gangouil on 7/24/24.
//

#include "irc.hpp"

void	Command::inviteFlag(bool input, Channel *channel, Client& client)
{
	std::string	mode;

	if (input != channel->getInviteOnly())
	{
		if (input == true)
			mode = "+i";
		else
			mode = "-i";
		channel->broadcastMsg(":" + client.GetNick() + "!" + client.GetUsername() + "@"
							  + Command::getHostname() + " MODE " + channel->getName() + " "
							  + mode + "\r\n", client, true);
	}
	channel->setInviteOnly(input);
}

void	Command::topicFlag(bool input, Channel *channel, Client& client)
{
	std::string	mode;

	if (input != channel->getTopicRestr())
	{
		if (input == true)
			mode = "+t";
		else
			mode = "-t";
		channel->broadcastMsg(":" + client.GetNick() + "!" + client.GetUsername() + "@"
							  + Command::getHostname() + " MODE " + channel->getName() + " "
							  + mode + "\r\n", client, true);
	}
	channel->setTopicRestr(input);
}

void	Command::keyFlag(bool input, Channel *channel, Client& client, std::string key)
{
	std::string	mode;

	if (key.empty())
	{
		if (input == true)
			client.sendErrorMsg(":ircserv " + (ERR_NEEDMOREPARAMS(client.GetNick(), "MODE +k")) + "\r\n");
		else
			client.sendErrorMsg(":ircserv " + (ERR_NEEDMOREPARAMS(client.GetNick(), "MODE -k")) + "\r\n");
		return ;
	}
	if (input != channel->getIsChannelKey()
	&& ((channel->getIsChannelKey() && key == channel->getKey())
	|| !channel->getIsChannelKey()))
	{
		if (input == true)
			mode = "+k";
		else
			mode = "-k";
		channel->broadcastMsg(":" + client.GetNick() + "!" + client.GetUsername() + "@"
							  + Command::getHostname() + " MODE " + channel->getName() + " "
							  + mode + " " + key + "\r\n", client, true);
		channel->setIsChannelKey(input);
		channel->setKey(key);
	}
	else
	{
		mode = ":ircserv ";
		channel->broadcastMsg(mode + ERR_KEYSET(client.GetNick(), channel->getName()) + "\r\n",
							  client, true);
	}
}

void	Command::operatorFlag(bool input, Server& server, Channel *channel, Client& client, std::string userName)
{
	std::string	mode;
	Client		*user;

	if (userName.empty())
		return ;
	user = server.getClientByName(userName, 1);
	if (!user)
	{
		client.sendErrorMsg(":ircserv " + (ERR_NOSUCHNICK(client.GetNick(), userName) + "\r\n"));
		return ;
	}
	if (channel->getUserList().count(client.GetFd()) == 0)
	{
		client.sendErrorMsg(":ircserv "
			+ (ERR_USERNOTINCHANNEL(client.GetNick(), userName, channel->getName())) + "\r\n");
		return ;
	}
	if ((input == true && channel->getOperators().count(user->GetFd()) != 0)
	|| (input == false && channel->getOperators().count(user->GetFd() == 0)))
		return ;

	if (input == true)
	{
		channel->addOperator(user);
		mode = "+o";
	}
	else
	{
		if (channel->getOperators().count(user->GetFd()) != 0)
			channel->delOperator(user);
		mode = "-o";
	}
	channel->broadcastMsg(":" + client.GetNick() + "!" + client.GetUsername() + "@"
						  + Command::getHostname() + " MODE " + channel->getName() + " "
						  + mode + " " + user->GetNick() + "\r\n",
						  client, true);
}

void	Command::limitFlag(bool input, Channel *channel, Client& client, std::string limit)
{
	std::string	mode;
	long		userLimit = 0;

	if (input == false)
	{
		if (channel->getUserLimit() == -1)
			return ;
		mode = "-l";
		channel->setUserLimit(-1);
		channel->broadcastMsg(":" + client.GetNick() + "!" + client.GetUsername() + "@"
							  + Command::getHostname() + " MODE " + channel->getName() + " "
							  + mode + "\r\n", client, true);
		return ;
	}
	mode = "+l";
	for (std::string::iterator it = limit.begin(); it != limit.end(); ++it)
	{
		if (it == limit.begin() && *it == '+') {
			continue ;
		}
		if (!isdigit(*it) || userLimit > INT_MAX)
			break ;
		userLimit = userLimit * 10 + (*it - '0');
	}
	if (userLimit > INT_MAX || userLimit <= 0 || userLimit == channel->getUserLimit())
		return ;
	std::ostringstream ss;
	ss << userLimit;
	channel->setUserLimit(userLimit);
	channel->broadcastMsg(":" + client.GetNick() + "!" + client.GetUsername() + "@"
						  + Command::getHostname() + " MODE " + channel->getName() + " "
						  + mode + " " + ss.str() + "\r\n", client, true);
}