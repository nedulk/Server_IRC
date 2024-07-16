/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:30:21 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/16 14:13:36 by kprigent         ###   ########.fr       */
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
	
	regex_t regex;
	while (1)
	{
		if (Server::_Signal == true)
		{
			ClearAllClients();
			CloseFds();
			exit(0);
		}
		size_t bytes = recv(fd_newClient, buff_r, sizeof(buff_r) - 1, 0);
		if (bytes <= 0)
		{
			std::cout << ITALIC "Client [" << fd_newClient << "]" RESET;
			std::cout << BRED " disconnected ×" RESET << std::endl;
			ClearClients(fd_newClient);
			close(fd_newClient);
			throw (std::runtime_error("Client disconected"));
		}
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
						std::cout << RED "Error: ERR_ERRONEUSNICKNAME " << "[" << newClient->GetIp() << "] ["
							<< newClient->GetFd() << "]" RESET << std::endl;
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
				std::cout << RED "Error: ERR_NICKNAMEINUSE " << "[" << newClient->GetIp() << "] ["
						<< newClient->GetFd() << "]" RESET << std::endl;
				for (int i = 0; i < 1024; i++)
					buff_r[i] = '\0';
				
			}
		}
	}
	regfree(&regex);
}

void Server::PasswordCheck(int fd_newClient)
{
	char buff_r[1024];
	for (int i = 0; i < 1024; i++)
		buff_r[i] = '\0';
	
	while (1)
	{
		if (Server::_Signal == true)
		{
			ClearAllClients();
			CloseFds();
			exit(0);
		}
		size_t bytes = recv(fd_newClient, buff_r, sizeof(buff_r) - 1, 0);
		if (bytes <= 0)
		{
			std::cout << ITALIC "Client [" << fd_newClient << "]" RESET;
			std::cout << BRED " disconnected ×" RESET << std::endl;
			ClearClients(fd_newClient);
			close(fd_newClient);
			throw (std::runtime_error("Client disconected"));
		}
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
				std::string message = std::string(RED).append(ERR_PASSWDMISMATCH).append("\n").append(RESET);
				send(fd_newClient, message.c_str(), message.size(), 0);
				std::cout << RED "Error: ERR_PASSWDMISMATCH [" << fd_newClient << "]" RESET << std::endl;
				for (int i = 0; i < 1024; i++)
					buff_r[i] = '\0';
			}
		}
	}	
}

void Server::UserCheck(int fd_newClient, Client *newClient)
{
    char buff_r[1024];
    for (int i = 0; i < 1024; i++)
        buff_r[i] = '\0';
    
    regex_t regex;
    while (1)
    {
        if (Server::_Signal == true)
		{
			ClearAllClients();
			CloseFds();
			exit(0);
		}
		size_t bytes = recv(fd_newClient, buff_r, sizeof(buff_r) - 1, 0);
		if (bytes <= 0)
		{
			std::cout << ITALIC "Client [" << fd_newClient << "]" RESET;
			std::cout << BRED " disconnected ×" RESET << std::endl;
			ClearClients(fd_newClient);
			close(fd_newClient);
			throw (std::runtime_error("Client disconected"));
		}
        int ret;
        ret = regcomp(&regex, USER, REG_EXTENDED);
		if (ret < 0)
		{
			regfree(&regex);
			return ;
		}
        if (!ret)
        {	
            ret = regexec(&regex, buff_r, 0, NULL, 0);
            if (!ret)
            {
                char *buff_rr = buff_r;
                while(*buff_rr != ':')
                    buff_rr++;
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
                newClient->SetUsername(buff_rr);
                regfree(&regex);
                break ;
            }
        }
    	regfree(&regex);
    }	
}

void Server::FirstCoHandler(int fd_newClient, Client *newClient)
{
	try
	{
		//PASS
		PasswordCheck(fd_newClient);
		std::cout << "PASS cmd [OK] [" << fd_newClient << "]" << std::endl;
	
		//NICK
		NickCheck(fd_newClient, newClient) ;
		std::cout << "NICK cmd [OK] [" << fd_newClient << "]" << std::endl;
	
		//USER
		UserCheck(fd_newClient, newClient);
		std::cout << "USER cmd [OK] [" << fd_newClient << "]" << std::endl;
	}
	catch (std::exception& e)
	{
		return ;
	}
	
	std::cout << ITALIC "New client [" << newClient->GetIp() << "]" << " [" << newClient->GetFd() << "]" RESET;
	std::cout << BGREEN " connected ✔️" RESET << std::endl;
	
	//WELCOME MSG -> to client 
	std::string message = std::string(GREEN) + RPL_WELCOME(newClient->GetNick(), newClient->GetUsername(), "hostname") + "\n"
		+ RPL_YOURHOST("RCI", "1.2.5") + "\n" + RPL_CREATED("today") + "\n" + RPL_MYINFO("RCI", "1.2.5", "i/t/k/o/l", "...") + "\n" + (RESET);
	send(fd_newClient, message.c_str(), message.size(), 0);
	std::cout << GREEN "Reply: RPL_WELCOME / RPL_YOURHOST / RPL_CREATED / RPL_MYINFO " << "[" << fd_newClient << "]" RESET << std::endl;
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
