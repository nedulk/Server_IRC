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
		for (std::vector<std::string>::iterator it = receiverNames.begin() + 1; it != receiverNames.end(); ++it)
		{
			std::string finalMsg = client.GetNick() + "!" + client.GetNick()
										+ "@hostname PRIVMSG " + *it + ": " + msg;
			Client* receiverClient = server.getClientByName(*it);

			send(receiverClient->GetFd(), finalMsg.c_str(), finalMsg.size(), 0);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}