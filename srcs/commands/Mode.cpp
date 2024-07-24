//
// Created by trcgab on 7/22/24.
//

#include "irc.hpp"

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

static void	clearEmptyArgs(std::vector<std::string>& args)
{
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it)
	{
		if ((*it).empty())
			args.erase(it);
	}
}

void	Command::modeCmd(Server &server, Client &client, std::vector<std::string> args)
{
	std::vector<std::pair<int, std::string> >	flags;
	std::vector<std::string>					cmdArgs;

	clearEmptyArgs(args);
	args.erase(args.begin());
	if (args.empty())
	{
		// ERR_NEEDMOREPARAMS
		return ;
	}
	getArgs(flags, cmdArgs, args);
	for (std::vector<std::pair<int, std::string> >::iterator it = flags.begin(); it != flags.end(); ++it)
	{
		std::cout << (*it).first << " : " << (*it).second << std::endl;
	}
	for (std::vector<std::string>::iterator it = cmdArgs.begin(); it != cmdArgs.end(); ++it)
	{
		std::cout << "args: " << *it << std::endl;
	}
	(void)server;
	(void)client;
}