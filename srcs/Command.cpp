//
// Created by gangouil on 7/11/24.
//

#include "irc.hpp"

std::string Command::RegexCmd(std::string buff)
{
	std::vector <std::string> Regex_vector = {NICK, USER, QUIT, PRIVMSG, JOIN,
		PART, MODE, TOPIC, INVITE, KICK, QUIT};
	
	regex_t regex;
	int ret;

	for (std::vector <std::string>::iterator it = Regex_vector.begin(); it != Regex_vector.end(); ++it)
	{
		ret = regcomp(&regex, it->c_str() , REG_EXTENDED);
		if (!ret)
		{	
			ret = regexec(&regex, buff.c_str(), 0, NULL, 0);
			if (ret == 0)
				return(*it);
		}
		regfree(&regex);
	}
	return ("");
}

void Command::execCmd(std::string cmd)
{
	if (cmd == QUIT)
	{
		std::cout << YELLOW "QUIT cmd detected" RESET << std::endl;
		return ;
	}
	else if(cmd == PRIVMSG)
	{
		std::cout << YELLOW "PRIVMSG cmd detected" RESET << std::endl;
		// privMsg(server, sender, args[0], args[1]);
		return ;
	}
	else if (cmd == JOIN)
	{	
		std::cout <<  YELLOW "JOIN cmd detected" RESET << std::endl;
		// privMsg(server, sender, args[0], args[1]);
		return ;
	}
	else if (cmd == MODE)
	{
		std::cout << YELLOW "MODE cmd detected" RESET << std::endl;
		return ;
	}
	else if (cmd == TOPIC)
	{
		std::cout << YELLOW "TOPIC cmd detected" RESET << std::endl;
		return ;
	}
	else if (cmd == INVITE)
	{
		std::cout << YELLOW "INVITE cmd detected" RESET << std::endl;
		return ;
	}
	else if (cmd == KICK)
	{
		std::cout << YELLOW "KICK cmd detected" RESET << std::endl;
		return ;
	}
}

