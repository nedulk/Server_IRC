//
// Created by trcgab on 7/19/24.
//

#include "irc_bonus.hpp"

static void parseArg(std::string &arg, std::vector<std::string>& channels)
{
    unsigned long   i;
    unsigned long   nameStart;

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
}

static int getArgs(Client& client, std::vector<std::string>& args,
    std::vector<std::string>& channels, std::vector<std::string>& users)
{
    std::string input;

    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it)
        input += *it + " ";

    args.erase(args.begin());
    if (args.empty())
    {
        client.sendErrorMsg(":ircserv " + (ERR_NEEDMOREPARAMS(client.GetNick(), input)) + "\r\n");
        return (0);
    }
    parseArg(args.front(), channels);
    if (channels.empty())
    {
        client.sendErrorMsg(":ircserv " + (ERR_NEEDMOREPARAMS(client.GetNick(), input)) + "\r\n");
        return (0);
    }
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if ((*it)[0] == '#')
            continue ;
        client.sendErrorMsg(":ircserv " + (ERR_NOSUCHCHANNEL(client.GetNick(), *it)) + "\r\n");
        return (0);
    }
    args.erase(args.begin());
    if (args.empty())
    {
        client.sendErrorMsg(":ircserv " + (ERR_NEEDMOREPARAMS(client.GetNick(), input)) + "\r\n");
        return (0);
    }
    parseArg(args.front(), users);
    if (users.empty() || (channels.size() != 1 && users.size() != channels.size()))
    {
        client.sendErrorMsg(":ircserv " + (ERR_NEEDMOREPARAMS(client.GetNick(), input)) + "\r\n");
        return (0);
    }
    args.erase(args.begin());
    return (1);
}


static void kickUsers(Server& server, Client& client,
    std::string& channelName, std::vector<std::string>& usersName, std::string& comment)
{
    try
    {
        Channel *channel = server.getChannelList().at(channelName);
        std::map<int, Client*>  userList = channel->getUserList();

        if (channel->getOperators().count(client.GetFd()) == 0)
        {
            client.sendErrorMsg(":ircserv " + (ERR_CHANOPRIVSNEEDED(client.GetNick(), channelName)) + "\r\n");
            return ;
        }
        if (userList.count(client.GetFd()) == 0)
        {
            client.sendErrorMsg(":ircserv " +
                (ERR_NOTONCHANNEL(client.GetNick(), channelName)) + "\r\n");
            return ;
        }
        for (std::vector<std::string>::iterator it = usersName.begin(); it != usersName.end(); ++it)
        {
            try
            {
                Client  *currentUser = server.getClientByName(*it, 0);
                if (userList.count(currentUser->GetFd()) == 0)
                {
                    client.sendErrorMsg(":ircserv " +
                        (ERR_USERNOTINCHANNEL(client.GetNick(), *it, channelName)) + "\r\n");

                    continue ;
                }
                channel->broadcastMsg(":" + client.GetNick() + "!"
                        + client.GetUsername() + "@"
                        + Command::getHostname() + " KICK "
                        + channelName + " " + *it + " " + comment + "\r\n",
						client, true);
                channel->delUser(currentUser);
                if (channel->getOperators().count(currentUser->GetFd()) != 0)
                    channel->delOperator(currentUser);
				if (channel->getUserCount() == 0)
				{
					server.deleteChannel(channelName);
					delete channel;
					return ;
				}
            }
            catch (std::exception& e)
            {
                client.sendErrorMsg(":ircserv " +
                        (ERR_NOSUCHNICK(client.GetNick(), *it)) + "\r\n");
            }
        }
    }
    catch (std::exception& e)
    {
        client.sendErrorMsg(":ircserv " +
                        (ERR_NOSUCHCHANNEL(client.GetNick(), channelName)) + "\r\n");
    }
}

void Command::kickCmd(Server &server, Client &client, std::vector<std::string> args)
{
    std::vector<std::string>    channels;
    std::vector<std::string>    users;
    std::string                 comment = client.GetNick();

    if (!getArgs(client, args, channels, users))
        return ;
    if (!args.empty())
        comment = args.front();
    if (channels.size() == 1)
    {
        kickUsers(server, client, channels.front(), users, comment);
        return ;
    }
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        std::vector<std::string>    currentUser(1, users.front());
        kickUsers(server, client, *it, currentUser, comment);
        users.erase(users.begin());
    }
}
