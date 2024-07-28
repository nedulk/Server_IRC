/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:41:17 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/28 17:07:11 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::quitCmd(Server& server, Client& client, std::vector<std::string> args)
{
	std::string message = "ERROR: you have been disconnected ";	
	std::vector<std::string>::iterator it = args.begin(); 
	
	while (it != args.end())
	{
		if (it == args.begin())
			message.append(*it);
		else
			message.append(" ").append(*it);
		it++;
	}
	message += "\r\n";
	std::vector<Channel*> channelList = client.getJoinedChannels();
	for (std::vector<Channel*>::iterator it = channelList.begin(); it != channelList.end(); ++it)
	{
		(*it)->broadcastMsg(":" + client.GetNick() + "!" + client.GetHostname()
				+ "@" + Command::getHostname() + " QUIT :" + message,
				client, false);
	}
	std::cout << ITALIC "New client [" << client.GetIp() << "]" << " [" << client.GetFd() << "]" RESET;
	std::cout << BRED " disconnected ❌" RESET << std::endl;
	(void)server;
	int tmp_fd = client.GetFd();
	server.ClearClients(tmp_fd);
	close(tmp_fd);
}
