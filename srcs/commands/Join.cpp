//
// Created by gangouil on 7/11/24.
//

#include "Command.hpp"

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
			else
				keys.push_back(arg.substr(nameStart, i - nameStart));
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
		if (arg[i] != '#')
		{
			channels.clear();
			return (channels);
		}
		while (arg[i] && arg[i] != ',')
			i++;
		if (nameStart == i && arg[nameStart] == '#')
			channels.push_back("#");
		else
			channels.push_back(arg.substr(nameStart, i - nameStart));
	}
	return (channels);
}

void Command::join(Server& server, Client& sender, std::vector<std::string> &args)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;

	args.erase(args.begin());
	channels = getJoinChannels(args.front());
	if (channels.empty())
	{
		std::cout << "channel name error" << std::endl;
		return ;
	}
	args.erase(args.begin());
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
	if (server.getChannelList().count(channelName) == 0) //channel doesnt exist
	{
		server.createChannel(&sender, channelName, key);
		return ;
	}
	try	//channel exists
	{
		Channel	*channel = server.getChannelList().at(channelName);

		std::map<int, Client*>	userList = channel->getUserList();
		if (userList.count(sender.GetFd()) != 0)
			return ;
		if (channel->getInviteOnly() && sender.isInvitedToChannel(channelName) == false)
			throw (std::runtime_error(ERR_INVITEONLYCHAN(channelName + " " + key, channelName)));
		if (channel->getIsUserLimit() && channel->getUserLimit() + 1 > channel->getUserCount())
			throw (std::runtime_error(ERR_CHANNELISFULL(channelName + " " + key, channelName)));
		if (channel->getIsChannelKey() && key != channel->getKey())
			throw (std::runtime_error(ERR_BADCHANNELKEY(channelName + " " + key, channelName)));
		channel->addUser(&sender);
		server.broadcastMsg(":" + sender.GetNick() + "!~" + sender.GetUsername() + "@" +
			getHostname() + " JOIN :" + channelName + "\r\n", channelName, sender, true);
//		std::cout << sender.GetNick() + " successfully joined " << channelName << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
