//
// Created by gangouil on 7/11/24.
//

#include "irc.hpp"

std::vector<std::string> Command::GetCmdArgs(std::string buff)
{
    std::vector<std::string> args;
    std::string::size_type start = 0;
    std::string::size_type end = buff.find(' ');

    while (end != std::string::npos) 
	{
        args.push_back(buff.substr(start, end - start));
        start = end + 1;
        end = buff.find(' ', start);
    }
    args.push_back(buff.substr(start));

	////// DEBUG pour voir les arguments d'une commande //////
	// std::cout << "cmd args: \n";
	// for(std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it)
	// {
	// 	std::cout << *it << std::endl;
	// }
    return (args);
}

std::string Command::RegexCmd(std::string buff)
{
	std::vector<std::string> Regex_vector;
	Regex_vector.push_back(NICK);
	Regex_vector.push_back(USER);
	Regex_vector.push_back(QUIT);
	Regex_vector.push_back(PRIVMSG);
	Regex_vector.push_back(JOIN);
	Regex_vector.push_back(MODE);
	Regex_vector.push_back(TOPIC);
	Regex_vector.push_back(INVITE);
	Regex_vector.push_back(KICK);
	Regex_vector.push_back(QUIT);
	
	regex_t regex;
	int ret;

	for (std::vector <std::string>::iterator it = Regex_vector.begin(); it != Regex_vector.end(); ++it)
	{
		ret = regcomp(&regex, it->c_str() , REG_EXTENDED);
		if (!ret)
		{	
			ret = regexec(&regex, buff.c_str(), 0, NULL, 0);
			if (ret == 0)
			{
				regfree(&regex);
				return(*it);
			}
		}
		regfree(&regex);
	}
	return ("");
}

void Command::execCmd(Server& server, Client& client, std::string cmdName, std::vector<std::string> args)
{
	if (cmdName == QUIT)
	{
		std::cout << YELLOW "QUIT cmd detected" RESET << std::endl;
		quitCmd(server, client, args);
		return ;
	}
	else if (cmdName == PRIVMSG)
	{
		std::cout << YELLOW "PRIVMSG cmd detected" RESET << std::endl;
		privMsg(server, client, args);
		return ;
	}
	else if (cmdName == JOIN)
	{	
		std::cout <<  YELLOW "JOIN cmd detected" RESET << std::endl;
		join(server, client, args);
		return ;
	}
	else if (cmdName == MODE)
	{
		std::cout << YELLOW "MODE cmd detected" RESET << std::endl;
		return ;
	}
	else if (cmdName == TOPIC)
	{
		std::cout << YELLOW "TOPIC cmd detected" RESET << std::endl;
		return ;
	}
	else if (cmdName == INVITE)
	{
		std::cout << YELLOW "INVITE cmd detected" RESET << std::endl;
		return ;
	}
	else if (cmdName == KICK)
	{
		std::cout << YELLOW "KICK cmd detected" RESET << std::endl;
		return ;
	}
}

std::string Command::getHostname()
{
	char		login[LOGIN_NAME_MAX];
	char		host[HOST_NAME_MAX];
	std::string	hostname;

	getlogin_r(login, LOGIN_NAME_MAX);
	gethostname(host, HOST_NAME_MAX);
	login[LOGIN_NAME_MAX - 1] = 0;
	host[HOST_NAME_MAX - 1] = 0;

	hostname = login;
	hostname += "@";
	hostname += host;
	return (hostname);
}