//
// Created by trcgab on 7/22/24.
//

#include "irc.hpp"

void	inviteUser(Server& server, Client& client, std::string& invitedNick, std::string& channelName)
{
	try
	{
		Channel	*channel = server.getChannelList().at(channelName);
		Client	*invitedClient = server.getClientByName(invitedNick);
		std::string	msg;

		if (channel->getUserList().count(client.GetFd()) == 0)
		{
			client.sendErrorMsg(":ircserv " +
				(ERR_NOTONCHANNEL(client.GetNick(), channelName)) + "\r\n");
			return ;
		}
		if (channel->getInviteOnly() == true && channel->getOperators().count(client.GetFd()) == 0)
		{
			client.sendErrorMsg(":ircserv " +
				(ERR_CHANOPRIVSNEEDED(client.GetNick(), channelName)) + "\r\n");
			return ;
		}
		if (channel->getUserList().count(invitedClient->GetFd()) != 0)
		{
			client.sendErrorMsg(":ircserv " +
				(ERR_USERONCHANNEL(client.GetNick(), invitedNick, channelName)) + "\r\n");
			return ;
		}
		channel->addInvite(invitedClient);
		msg = ":" + client.GetNick() + "!" + client.GetUsername() + "@" +
			Command::getHostname() + " INVITE " + invitedNick + " " + channelName + "\r\n";
		send(invitedClient->GetFd(), msg.c_str(), msg.size(), 0);

		msg = ":ircserv 341 " + client.GetNick() + " " + invitedNick + " "
			+ channelName + "\r\n";
		send(client.GetFd(), msg.c_str(), msg.size(), 0);
	}
	catch (std::exception& e)
	{
		client.sendErrorMsg(":ircserv " +
				(ERR_NOSUCHNICK(client.GetNick(), invitedNick)) + "\r\n");
		// if (e.what() == "Client not found")
			// ; // ERR_NOSUCHNICK
		// else {
		// }; // ERR_NOSUCHCHANNEL
	}
}

void Command::inviteCmd(Server &server, Client &client, std::vector<std::string> args)
{
	std::string	invitedNick;
	std::string	channelName;
	std::string input;

	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it)
		input += *it + " ";
	args.erase(args.begin());
	if (args.empty())
	{
		client.sendErrorMsg(":ircserv " +
				(ERR_NEEDMOREPARAMS(client.GetNick(), input)) + "\r\n");
		return ;
	}
	invitedNick = args.front();
	args.erase(args.begin());
	if (args.empty())
	{
		client.sendErrorMsg(":ircserv " +
				(ERR_NEEDMOREPARAMS(client.GetNick(), input)) + "\r\n");
		return ;
	}
	channelName = args.front();
	inviteUser(server, client, invitedNick, channelName);
}
