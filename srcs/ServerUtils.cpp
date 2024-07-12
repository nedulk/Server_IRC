/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:30:21 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/12 15:21:35 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	Server::NickCheck_oc(std::string buff_rr)
{
	if (!_Clients.empty())
	{
		for(std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
		{
			if ((*it)->GetNick() == buff_rr)
				return (1);
		}
	}
	return (0);
}

void Server::NickCheck(int fd_newClient, Client *newClient)
{
	char buff_r[1024];
	for (int i = 0; i < 1024; i++)
		buff_r[i] = '\0';
	
	while (1)
	{
		recv(fd_newClient, buff_r, sizeof(buff_r) - 1, 0);
		if (std::strncmp(buff_r, "NICK ", 5) == 0)
		{
			char *buff_rr = buff_r + 5;
			while (*buff_rr == ' ')
				buff_rr++;
			char *p = buff_rr;
			while (*p != '\0')
			{
				if (*p == '\n')
				{
					*p = '\0';
					break;
				}
				p++;
			}
			if (NickCheck_oc(buff_rr) == 0)
			{	
				regex_t regex;
				int ret;
				ret = regcomp(&regex, NICK, REG_EXTENDED);
				if (!ret)
				{	
					ret = regexec(&regex, buff_rr, 0, NULL, 0);
					if (!ret)
					{
						newClient->SetNick(buff_rr);
						break ;
					}
					else
					{
						std::string buff_rr_str(buff_rr);
						std::string message = std::string(RED).append(ERR_ERRONEUSNICKNAME(buff_rr_str)).append("\n").append(RESET);
						send(fd_newClient, message.c_str(), message.size(), 0);
						for (int i = 0; i < 1024; i++)
							buff_r[i] = '\0';
					}
				}
				regfree(&regex); 
			}
			else
			{
				std::string buff_rr_str(buff_rr);
				std::string message = std::string(RED).append(ERR_NICKNAMEINUSE(buff_rr_str)).append("\n").append(RESET);
				send(fd_newClient, message.c_str(), message.size(), 0);
				for (int i = 0; i < 1024; i++)
					buff_r[i] = '\0';
				
			}
		}
	}
}

void Server::PasswordCheck(int fd_newClient)
{
	char buff_r[1024];
	for (int i = 0; i < 1024; i++)
		buff_r[i] = '\0';
	
	while (1)
	{
		if (Server::_Signal == true)
			exit(0);
		recv(fd_newClient, buff_r, sizeof(buff_r) - 1, 0);
		if (std::strncmp(buff_r, "PASS ", 5) == 0)
		{
			char *buff_rr = buff_r + 5;
			while (*buff_rr == ' ')
				buff_rr++;
			char *p = buff_rr;
			while (*p != '\0')
			{
				if (*p == '\n')
				{
					*p = '\0';
					break;
				}
				p++;
			}
			if(std::strcmp(buff_rr, this->_Password.c_str()) == 0)
				break;
			else 
			{
				std::string buff_rr_str(buff_rr);
				std::string message = std::string(RED).append(ERR_PASSWDMISMATCH(buff_rr_str)).append("\n").append(RESET);
				send(fd_newClient, message.c_str(), message.size(), 0);
				for (int i = 0; i < 1024; i++)
					buff_r[i] = '\0';
			}
		}
	}	
}

void Server::FirstCoHandler(int fd_newClient, Client *newClient)
{
	//PASS
	PasswordCheck(fd_newClient);
	std::cout << "PASS cmd [OK]" << std::endl;
	
	//NICK
	NickCheck(fd_newClient, newClient) ;
	std::cout << "NICK cmd [OK]" << std::endl;
	
	//USER
	newClient->UserCheck(fd_newClient);
	std::cout << "USER cmd [OK]" << std::endl;

	std::cout << ITALIC "New client [" << newClient->GetIp() << "]" << " [" << newClient->GetFd() << "]" RESET;
	std::cout << BGREEN " connected ✔️" RESET << std::endl;
	
	//WELCOME MSG -> to client 
	std::string str(newClient->GetNick());
	std::string message = std::string(GREEN).append(RPL_WELCOME(str)).append("\n").append(RESET);
	send(fd_newClient, message.c_str(), message.size(), 0);
}

std::map<std::string, Channel*> Server::getChannelList() {
	return (_channelList);
}

void Server::createChannel(Client *oper, std::string &channelName, std::string key)
{
	try
	{
		Channel	*newChannel = new Channel(channelName);

		if (!key.empty())
		{
			newChannel->setKey(key);
			newChannel->setIsChannelKey(true);
		}
		newChannel->addOperator(oper);
		newChannel->addUser(oper);
		_channelList[channelName] = newChannel;
		std::cout << "Channel " + channelName + "successfully created" << std::endl;
		std::cout << "User " + oper->GetNick() + "successfully joined" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
