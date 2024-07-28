//
// Created by gangouil on 7/11/24.
//

#include "Command_bonus.hpp"

std::vector<std::string> Command::getJoinKeys(std::string &arg)
{
	std::vector<std::string> keys;
	unsigned long	i;
	unsigned long	nameStart;

	i = 0;
	nameStart = 0;
	while (arg[i])
	{
		if (arg[i] == ',' || !arg[i + 1])
		{
			if (arg[nameStart] == ',')
				keys.push_back("");
			else if (!arg[i + 1] && arg[i] != ',')
				keys.push_back(arg.substr(nameStart, i - nameStart + 1));
			else
				keys.push_back(arg.substr(nameStart, i - nameStart));
			if (!arg[i + 1] && arg[i] == ',')
				keys.push_back("");
			nameStart = i + 1;
		}
		i++;
	}
	return (keys);
}

std::vector<std::string> Command::getJoinChannels(std::string &arg)
{
	std::vector<std::string>	channels;
	unsigned long				i;
	unsigned long				nameStart;

	i = 0;
	while (arg[i])
	{
		while (arg[i] == ',')
			i++;
		nameStart = i;
		if (!arg[i])
			break;
		while (arg[i] && arg[i] != ',')
			i++;
		channels.push_back(arg.substr(nameStart, i - nameStart));
	}
	return (channels);
}

void Command::joinCmd(Server& server, Client& sender, std::vector<std::string> &args)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;

	args.erase(args.begin());
	channels = getJoinChannels(args.front());
	if (channels.empty())
	{
		sender.sendErrorMsg(":ircserv " + (ERR_NEEDMOREPARAMS(sender.GetNick(), "JOIN")) + "\r\n");
		return ;
	}
	args.erase(args.begin());
	if (!args.empty())
		keys = getJoinKeys(args.front());
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (keys.empty())
			joinChannel(server, sender, *it, "");
		else
		{
			joinChannel(server, sender, *it, keys.front());
			keys.erase(keys.begin());
		}
	}
}

void Command::joinChannel(Server& server, Client& sender, std::string &channelName, std::string key)
{
	if (server.getChannelList().count(channelName) == 0)
	{
		server.createChannel(&sender, channelName, key);
		return ;
	}
	try
	{
		Channel	*channel = server.getChannelList().at(channelName);

		std::map<int, Client*>	userList = channel->getUserList();
		if (userList.count(sender.GetFd()) != 0)
			return ;
		if (channel->getInviteOnly() && channel->isInvited(sender.GetFd()) == false)
			throw (std::runtime_error(ERR_INVITEONLYCHAN(sender.GetNick(), channelName)));
		if (channel->getUserLimit() != -1 && channel->getUserLimit() <= channel->getUserCount())
			throw (std::runtime_error(ERR_CHANNELISFULL(sender.GetNick(), channelName)));
		std::cout << "key = '" << key << "', chanKey = '" << channel->getKey() << "'" << std::endl;
		if (channel->getIsChannelKey() && key != channel->getKey())
			throw (std::runtime_error(ERR_BADCHANNELKEY(sender.GetNick(), channelName)));

		channel->addUser(&sender);
		if (channel->isInvited(sender.GetFd()))
			channel->delInvite(sender.GetFd());
		sender.joinedChannel(channel);

		if (!channel->getTopic().empty())
		{
			std::string topicMsg = RPL_TOPIC(sender.GetNick(), channel->getName(), channel->getTopic()) + "\r\n";
			send(sender.GetFd(), topicMsg.c_str(), topicMsg.size(), 0);
		}
		channel->broadcastMsg(":" + sender.GetNick() + "!" + sender.GetUsername() + "@" +
			getHostname() + " JOIN " + channelName + " * :" + sender.GetRealname() + "\r\n",
			sender, true);
		channel->broadcastUserList(sender);
	}
	catch (std::exception& e)
	{
		std::string msg = ":ircserv ";
		msg += e.what();
		sender.sendErrorMsg(msg + "\r\n");
	}
}
