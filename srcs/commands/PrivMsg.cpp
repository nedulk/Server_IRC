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
				{
					std::string receivers = it->substr(0, pos);
					size_t start = 0;
					size_t end = receivers.find(",");
					while (end != std::string::npos)
					{
						receiverNames.push_back(receivers.substr(start, end - start));
						start = end + 1;
						end = receivers.find(",", start);
					}
					receiverNames.push_back(receivers.substr(start));
				}
				msg = it->substr(pos + 1);
				isMsg = true;
				if ((it + 1) != args.end())
					msg += " ";
			}
			else
			{
				std::string receivers = *it;
				size_t start = 0;
				size_t end = receivers.find(",");
				while (end != std::string::npos)
				{
					receiverNames.push_back(receivers.substr(start, end - start));
					start = end + 1;
					end = receivers.find(",", start);
				}
				receiverNames.push_back(receivers.substr(start));
			}
		}
		bool onlyWhitespace = true;
		for (std::string::const_iterator it = msg.begin(); it != msg.end(); ++it)
		{
			if (*it != ' ' && *it != '\t' && *it != '\n' && *it != '\r')
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
			for (std::string::size_type i = 0; i < msg.size(); ++i)
			{
				if (msg[i] == '\n' || msg[i] == '\r')
				{
					msg.erase(i--, 1);
				}
			}
			for (std::vector<std::string>::iterator it = receiverNames.begin() + 1; it != receiverNames.end(); ++it)
			{
				std::string finalMsg =  client.GetNick() + "!" + getHostname()
						+ " PRIVMSG " + *it + ": " + msg + "\n" + "\r\n";
				if ((*it)[0] == '#')
				{
					server.broadcastMsg(finalMsg, *it, client, false);
				}
				else
				{
					Client* receiverClient = server.getClientByName(*it, 1);
					if (receiverClient == NULL)
					{
						std::string err_nick = ERR_NOSUCHNICK(*it);
						send(client.GetFd(), err_nick.c_str(), (ERR_NOSUCHNICK(*it)).size(), 0);
						std::cout << RED "Error: ERR_NOSUCHNICK " << "[" << client.GetIp() << "] ["
							<< client.GetFd() << "]" RESET << std::endl;
						throw(std::runtime_error (""));
					}
					send(receiverClient->GetFd(), finalMsg.c_str(), finalMsg.size(), 0);
				}
			}
			std::cout << GREEN "-> Message '" << msg << "' sent to " << receiverNames.size() - 1 << " targets." RESET << std::endl;
		}
	}
	catch (std::exception& e)
	{
	}
}