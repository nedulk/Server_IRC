//
// Created by trcgab on 7/20/24.
//

#include "irc.hpp"

static int  getChannels(std::string &arg, std::vector<std::string>& channels)
{
	unsigned long   i;
	unsigned long   nameStart;

	i = 0;
	nameStart = 0;
	while (arg[i])
	{
		while (arg[i] == ',')
			i++;
		if (!arg[i])
			break ;
		while (arg[i] && arg[i] != ',')
			i++;
		channels.push_back(arg.substr(nameStart, i - nameStart));
	}
	return (1);
}

static void	printUserList(Server& server, Client& client, std::string& channelName)
{
	try
	{
		Channel* channel = server.getChannelList().at(channelName);
		std::map<int, Client*> userList = channel->getUserList();
		std::string	msg;

		for (std::map<int, Client*>::iterator it = userList.begin(); it != userList.end(); ++it)
		{
			msg = ":ircserv ";
			msg += RPL_WHOREPLY(client.GetNick(), channelName,
					it->second->GetUsername() + " " + it->second->GetHostname()) + " "
					+ Command::getHostname() + " "
					+ it->second->GetNick() + " H";
			if (channel->getOperators().count(it->second->GetFd()) == 1)
				msg += "@";
			msg += "xz :0 " + it->second->GetRealname() + "\r\n";
			send(client.GetFd(), msg.c_str(), msg.size(), 0);
		}
		msg = ":ircserv ";
		msg += RPL_ENDOFWHO(client.GetNick(), channelName) + "\r\n";
		send(client.GetFd(), msg.c_str(), msg.size(), 0);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Command::whoCmd(Server &server, Client &client, std::vector<std::string> args)
{
	std::vector<std::string>	channels;

	args.erase(args.begin());
	if (args.empty())
		return ;
	getChannels(args.front(), channels);
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		printUserList(server, client, *it);
	}
}