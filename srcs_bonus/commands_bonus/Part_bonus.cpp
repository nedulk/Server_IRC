//
// Created by trcgab on 7/19/24.
//

#include "irc.hpp"

static int  getChannels(std::string &arg, std::vector<std::string>& channels)
{
    unsigned long   i;
    unsigned long   nameStart;

    i = 0;
    nameStart = 0;
    if (arg[0] == ',')
        return (0);
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

static void partUser(Server &server, Client &client, std::string& channelName, std::string& reason)
{
    try
    {
        Channel *channel = server.getChannelList().at(channelName);

        if (channel->getUserList().count(client.GetFd()) == 0)
        {
            std::string msg = ERR_NOTONCHANNEL(client.GetNick(), channelName) + "\r\n";
            send(client.GetFd(), msg.c_str(), msg.size(), 0);
            return ;
        }
        channel->broadcastMsg(":" + client.GetNick() + "!" + client.GetUsername() + "@"
            + Command::getHostname() + " PART " + channelName + " " + reason + "\r\n",
			client, true);
        channel->delUser(&client);
        if (channel->getOperators().count(client.GetFd()) != 0)
            channel->delOperator(&client);
        client.leftChannel(channel);
        if (channel->getUserCount() == 0)
        {
            server.deleteChannel(channelName);
            delete (channel);
        }
    }
    catch (std::exception& e)
    {
        (void)e;
        std::string msg = ERR_NOSUCHCHANNEL(client.GetNick(), channelName) + "\r\n";
        send(client.GetFd(), msg.c_str(), msg.size(), 0);
    }
}

void Command::partCmd(Server &server, Client &client, std::vector<std::string> args)
{
    std::vector<std::string>    channels;
    std::string                 reason = "Leaving";

    args.erase(args.begin());
    if (args.empty())
    {
        std::string msg = ERR_NEEDMOREPARAMS(client.GetNick(), "PART") + "\r\n";
        send(client.GetFd(), msg.c_str(), msg.size(), 0);
        return ;
    }
    if (getChannels(args.front(), channels) == 0)
        return ;
    args.erase(args.begin());
    if (!args.empty())
        reason = args.front();
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
        partUser(server, client, *it, reason);
}