//
// Created by gangouil on 7/11/24.
//

#include "irc.hpp"

void Command::execCmd(Server& server, int cmdName, Client& sender, std::vector<std::string> args)
{
	switch (cmdName)
	{
		case QUIT:
			break;
		case PRIVMSG:
			privMsg(server, sender, args[0], args[1]);
			break;
		case JOIN:
			privMsg(server, sender, args[0], args[1]);
			break;
		case PART:
			break;
		case MODE:
			break;
		case TOPIC:
			break;
		case INVITE:
			break;
		case KICK:
			break;
		default:
			std::cerr << RED "Command not found" RESET << std::endl;
	}
}

//void Command::parseCmd(std::string args)
//{
//
//}
