//
// Created by gangouil on 7/11/24.
//

#include "Command.hpp"

void Command::privMsg(Server& server, Client& client, std::vector<std::string> args)
{
	try
	{
		std::vector<std::string> receiverNames;
		std::string msg;
		bool isMsg = false;

		for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it)
		{
			size_t pos = it->find(":");
			if (isMsg)
			{
				msg += *it;
				if ((it + 1) != args.end())
					msg += " ";
			}
			else if (pos != std::string::npos)
			{
				if (pos > 0)
					receiverNames.push_back(it->substr(0, pos));
				msg = it->substr(pos + 1);
				isMsg = true;
				msg += " ";
			}
			else
				receiverNames.push_back(*it);
		}
		bool onlyWhitespace = true;
		for (std::string::const_iterator it = msg.begin(); it != msg.end(); ++it)
		{
			if (*it != ' ' && *it != '\t' && *it != '\n')
			{
				onlyWhitespace = false;
				break;
			}
		}
		if (onlyWhitespace)
		{
			send(client.GetFd(), ERR_NOTEXTTOSEND, strlen(ERR_NOTEXTTOSEND), 0);
			std::cout << RED "Error: ERR_NOTEXTTOSEND " << "[" << client.GetIp() << "] ["
				<< client.GetFd() << "]" RESET << std::endl;
		}
		else
		{
			for (std::vector<std::string>::iterator it = receiverNames.begin() + 1; it != receiverNames.end(); ++it)
			{
				std::string finalMsg =  client.GetNick() + "!" + getHostname()
						+ " PRIVMSG " + *it + ": " + msg + "\r\n";
				if ((*it)[0] == '#')
				{
					server.broadcastMsg(finalMsg, *it, client, false);
				}
				else
				{
					Client* receiverClient = server.getClientByName(*it);
					send(receiverClient->GetFd(), finalMsg.c_str(), finalMsg.size(), 0);
				}
			}
			for (std::string::size_type i = 0; i < msg.size(); ++i)
			{
				if (msg[i] == '\n')
				{
					msg.erase(i--, 1);
				}
			}
			std::cout << GREEN "-> Message '" << msg << "' sent to " << receiverNames.size() - 1 << " targets." RESET << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}