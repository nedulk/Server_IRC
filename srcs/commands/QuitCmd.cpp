/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:41:17 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/16 14:47:55 by kprigent         ###   ########.fr       */
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
	message.append(RESET);
	send(client.GetFd(), message.c_str(), message.size(), 0);
	std::cout << ITALIC "New client [" << client.GetIp() << "]" << " [" << client.GetFd() << "]" RESET;
	std::cout << BRED " disconnected ❌" RESET << std::endl;
	server.ClearClients(client.GetFd());
	close(client.GetFd());
}
