//
// Created by gangouil on 7/11/24.
//

#include "Command.hpp"

void Command::join(Server& server, Client* sender, std::string &channelName, std::string key)
{
	if (server.getChannelList().count(channelName) == 0) //channel doesnt exist
	{
		server.createChannel(sender, channelName, key);
		return ;
	}
	try	//channel exists
	{
		Channel	*channel = server.getChannelList().at(channelName);

		std::map<int, Client*>	userList = channel->getUserList();
		if (userList.count(sender->GetFd()) != 0)
			return ;
		if (channel->getInviteOnly() && sender->isInvitedToChannel(channelName) == false)
			throw (std::runtime_error(ERR_INVITEONLYCHAN(channelName + " " + key, channelName)));
		if (channel->getIsUserLimit() && channel->getUserLimit() + 1 > channel->getUserCount())
			throw (std::runtime_error(ERR_CHANNELISFULL(channelName + " " + key, channelName)));
		if (channel->getIsChannelKey() && key != channel->getKey())
			throw (std::runtime_error(ERR_BADCHANNELKEY(channelName + " " + key, channelName)));
		channel->addUser(sender);
		std::cout << sender->GetNick() + " successfully joined " << channelName << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}