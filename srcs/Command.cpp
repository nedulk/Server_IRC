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
	
    return (args);
}

std::string Command::RegexCmd(std::string buff)
{
	std::vector<std::string> Regex_vector;
	Regex_vector.push_back(QUIT);
	Regex_vector.push_back(PRIVMSG);
	Regex_vector.push_back(PRIVMSG2);
	Regex_vector.push_back(JOIN);
	Regex_vector.push_back(MODE);
	Regex_vector.push_back(TOPIC);
	Regex_vector.push_back(INVITE);
	Regex_vector.push_back(KICK);
	Regex_vector.push_back(QUIT);
	Regex_vector.push_back(PART);
	Regex_vector.push_back(WHO);
	
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
	}
	else if(cmdName == PRIVMSG || cmdName == PRIVMSG2)
	{
		std::cout << YELLOW "PRIVMSG cmd detected" RESET << std::endl;
		privMsg(server, client, args);
	}
	else if (cmdName == JOIN)
	{	
		std::cout <<  YELLOW "JOIN cmd detected" RESET << std::endl;
		joinCmd(server, client, args);
	}
	else if (cmdName == MODE)
	{
		std::cout << YELLOW "MODE cmd detected" RESET << std::endl;
		modeCmd(server, client, args);
	}
	else if (cmdName == TOPIC)
	{
		std::cout << YELLOW "TOPIC cmd detected" RESET << std::endl;
		topicCmd(server, client, args);
	}
	else if (cmdName == INVITE)
	{
		std::cout << YELLOW "INVITE cmd detected" RESET << std::endl;
		inviteCmd(server, client, args);
	}
	else if (cmdName == KICK)
	{
		std::cout << YELLOW "KICK cmd detected" RESET << std::endl;
		kickCmd(server, client, args);
	}
	else if (cmdName == PART)
	{
		std::cout << YELLOW "PART cmd detected" RESET << std::endl;
		partCmd(server, client, args);
	}
	else if (cmdName == WHO)
	{
		std::cout << YELLOW "WHO cmd detected" RESET << std::endl;
		whoCmd(server, client, args);
	}
}

std::string Command::getHostname()
{
	char		host[HOST_NAME_MAX];

	gethostname(host, HOST_NAME_MAX);
	host[HOST_NAME_MAX - 1] = 0;
	return (host);
}