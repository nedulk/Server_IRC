//
// Created by trcgab on 7/22/24.
//

#include "irc.hpp"

// reminder to limit all text inputs in size

void static	printTopic(Channel* channel, Client& client)
{
	std::string	msg = ":ircserv ";
	if (channel->getTopic().empty())
		msg += RPL_NOTOPIC(client.GetNick(), channel->getName()) + "\r\n";
	else
		msg += RPL_TOPIC(client.GetNick(), channel->getName(), channel->getTopic()) + "\r\n";
	send(client.GetFd(), msg.c_str(), msg.size(), 0);
}

void static setNewTopic(Server& server, Client& client, Channel* channel, std::vector<std::string>& args)
{
	std::string msg;
	std::string	topic;

	args.erase(args.begin());
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it)
	{
		if (topic.size() + (*it).size() > 400)
			break ;
		topic += *it + " ";
	}
	topic.erase(topic.size() - 1);
	if (topic == ":")
		topic.clear();
	channel->setTopic(topic);
	server.broadcastMsg(":" + client.GetNick() + "!" + client.GetUsername() + "@" +
				Command::getHostname() + " TOPIC " + channel->getName() + " " + topic + "\r\n",
				channel->getName(), client, true);
}

void Command::topicCmd(Server &server, Client &client, std::vector<std::string> args)
{
	args.erase(args.begin());
	if (args.empty())
	{
		client.sendErrorMsg(":ircserv " + (ERR_NEEDMOREPARAMS(client.GetNick(), "TOPIC")) + "\r\n");
		return ;
	}
	try
	{
		Channel	*channel = server.getChannelList().at(args.front());

		if (channel->getUserList().count(client.GetFd()) == 0)
		{
			client.sendErrorMsg(":ircserv " + (ERR_NOTONCHANNEL(client.GetNick(), args.front())) + "\r\n");
			return ;
		}
		if (channel->getTopicRestr() && channel->getOperators().count(client.GetFd()) == 0)
		{
			client.sendErrorMsg(":ircserv " + (ERR_CHANOPRIVSNEEDED(client.GetNick(), args.front())) + "\r\n");
			return ;
		}
		if (args.size() == 1)
			printTopic(channel, client);
		else
			setNewTopic(server, client, channel, args);
	}
	catch (std::exception& e)
	{
		client.sendErrorMsg(":ircserv " + (ERR_NOTONCHANNEL(client.GetNick(), args.front())) + "\r\n");
	}
}
