//
// Created by trcgab on 7/22/24.
//

 #include "irc.hpp"

static bool	isEmpty(std::string arg)
{
	return (arg.empty());
}

static int	getFlags(std::vector<std::pair<int, std::string> >& flags, std::string& arg)
{
	std::string	flagGroup;
	char		lastMod = '+';
	int 		neededArgs = 0;

	neededArgs = 0;
	for (std::string::iterator it = arg.begin(); it != arg.end(); ++it)
	{
		while (it != arg.end() && (*it == '+' || *it == '-'))
		{
			lastMod = *it;
			++it;
		}
		if (it == arg.end()) // only mods, no flag, behaviour TBD
			break ;
		flagGroup.push_back(lastMod);
		flagGroup.push_back(*it);
		if (*it == 'o' || *it == 'k' || *it == 'l')
			neededArgs++;
	}
	flags.push_back(std::pair<int, std::string>(neededArgs, flagGroup));
	return (neededArgs);
}

static void	getArgs(std::vector<std::pair<int, std::string> >& flags, std::vector<std::string>& cmdArgs,
	 std::vector<std::string>& args)
{
	std::string	flagGroup;
	bool		isFlag = true;
	int 		neededArgs = 0;

	while (!args.empty())
	{
		if (isFlag)
		{
			neededArgs = getFlags(flags, args.front());
			isFlag = false;
		}
		else
		{
			cmdArgs.push_back(args.front());
			neededArgs--;
		}
		args.erase(args.begin());
		if (neededArgs == 0)
			isFlag = true;
	}
}

void	Command::callModes(Server& server, Client& client, Channel* channel, std::string flag, std::string& arg)
{
	bool	modifier = true;

	if (flag[0] == '-')
		modifier = false;
	if (flag[1] == 'l')
		limitFlag(modifier, channel, client, arg);
	else if (flag[1] == 'o')
		operatorFlag(modifier, server, channel, client, arg);
	else if (flag[1] == 'k')
		keyFlag(modifier, channel, client, arg);
	else if (flag[1] == 't')
		topicFlag(modifier, channel, client);
	else if (flag[1] == 'i')
		inviteFlag(modifier, channel, client);
	else
		client.sendErrorMsg(":ircserv " + (ERR_UNKNOWNMODE(client.GetNick(), flag[1])) + "\r\n");
}

void	Command::executeModes(Server& server, Client& client, Channel* channel, std::pair<int, std::string>& flags,
							std::vector<std::string>& cmdArgs)
{
	std::string	flag;
	std::string	arg;
	std::string	msg;
	size_t 		i = 0;

	while (i < flags.second.size() - 1)
	{
		arg.clear();
		flag.clear();
		flag.push_back(flags.second[i]);
		flag.push_back(flags.second[i + 1]);
		if (flag[1] == 'o' || flag[1] == 'k' || flag[1] == 'l')
		{
			if (cmdArgs.empty())
				arg.clear();
			else
			{
				arg = cmdArgs.front();
				cmdArgs.erase(cmdArgs.begin());
			}
		}
		if (channel->getOperators().count(client.GetFd()) == 0)
		{
			msg = ":ircserv ";
			msg += ERR_CHANOPRIVSNEEDED(client.GetNick(), channel->getName()) + "\r\n";
			send(client.GetFd(), msg.c_str(), msg.size(), 0);
			i += 2;
			continue ;
		}
		callModes(server, client, channel, flag, arg);
		i += 2;
	}
}

void	Command::modeCmd(Server &server, Client &client, std::vector<std::string> args)
{
	std::vector<std::pair<int, std::string> >	flags;
	std::vector<std::string>					cmdArgs;
	std::string									msg;
	Channel										*channel;


	args.erase(std::remove_if(args.begin(), args.end(), isEmpty), args.end());
	args.erase(args.begin());
	if (args.empty())
	{
		// need to print active modes if no argument
		// shouldn't send an error msg
		client.sendErrorMsg(":ircserv " + (ERR_NEEDMOREPARAMS(client.GetNick(), "MODE")) + "\r\n");
		return ;
	}
	if (server.getChannelList().count(args.front()) == 0)
	{
		client.sendErrorMsg(":ircserv " + (ERR_NOSUCHCHANNEL(client.GetNick(), args.front())) + "\r\n");
		return ;
	}
	channel = server.getChannelList().at(args.front());
	args.erase(args.begin());
	getArgs(flags, cmdArgs, args);
	for (std::vector<std::pair<int, std::string> >::iterator it = flags.begin(); it != flags.end(); ++it)
	{
		std::cout << (*it).first << " : " << (*it).second << std::endl;
	}
	for (std::vector<std::string>::iterator it = cmdArgs.begin(); it != cmdArgs.end(); ++it)
	{
		std::cout << "args: " << *it << std::endl;
	}
	for (std::vector<std::pair<int, std::string> >::iterator it = flags.begin(); it != flags.end(); ++it)
	{
		executeModes(server, client, channel, *it, cmdArgs);
	}
}
