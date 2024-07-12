//
// Created by gangouil on 7/11/24.
//

#include "Command.hpp"

void Command::privMsg(Server& server, Client* sender, std::string &receiverName, std::string &msg)
{
	//needs to check if receiver is channel or user
	try
	{
		std::string	finalMsg = sender->GetNick() + "!" + sender->GetNick()
								  + "@hostname PRIVMSG " + sender->GetNick() + " : " + msg;
		Client* receiverClient = server.getClientByName(receiverName);

		send(receiverClient->GetFd(), finalMsg.c_str(), finalMsg.size(), 0);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}