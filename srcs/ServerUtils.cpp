/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:30:21 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/17 15:52:33 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::splitAndKeepLastTwo(const std::string& str)
{
    char* cstr = new char[str.length() + 1];
    std::strcpy(cstr, str.c_str());

    char* token = std::strtok(cstr, "\n\r");
    std::string lastToken;
    while (token != NULL)
    {
        lastToken = token;
        token = std::strtok(NULL, "\n\r");
    }
    if (!lastToken.empty())
        remain_line = lastToken;
    delete[] cstr;
}

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
			splitAndKeepLastTwo(buff_r);
			char *buff_rr;
			buff_rr = buff_r + 5;
			while (*buff_rr == ' ')
				buff_rr++;
			char *p = buff_rr;
			while (*p != '\0')
			{
				if (*p == '\n' || *p == '\r')
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
						std::string message = ERR_ERRONEUSNICKNAME(buff_rr_str);
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
				std::string message = ERR_NICKNAMEINUSE(buff_rr_str);
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
				if (*p == '\n' || *p == '\r')
				{
					*p = '\0';
					break;
				}
				p++;
			}
			if(std::strcmp(buff_rr, this->_Password.c_str()) == 0)
			{	
				for (int i = 0; i < 1024; i++)
					buff_r[i] = '\0';
				break;
			}
			else 
			{
				std::string buff_rr_str(buff_rr);
				std::string message = ERR_PASSWDMISMATCH;
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
			throw (std::runtime_error("Client disconnected"));
		}
		if (std::strncmp(buff_r, "USER ", 5) == 0 || std::strncmp(remain_line.c_str(), "USER ", 5) == 0 )
        {
			char *buff_rr;
			if (std::strncmp(remain_line.c_str(), "USER ", 5) == 0)
			{
				buff_rr = (char *)remain_line.c_str() + 5;
			}
			else
				buff_rr = buff_r + 5;
			while (*buff_rr == ' ')
				buff_rr++;
			char *p = buff_rr;
			while (*p != '\0')
			{
				if (*p == '\n' || *p == '\r')
				{
					*p = '\0';
					break;
				}
				p++;
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
				ret = regexec(&regex, buff_rr, 0, NULL, 0);
				if (!ret)
				{
					char *buff_rrr = buff_rr;
					while(*buff_rrr != ':')
						buff_rrr++;
					buff_rrr++;
					char *p = buff_rrr;
					while (*p != '\0')
					{
						if (*p == '\n' || *p == '\r')
						{
							*p = '\0';
							break;
						}
						p++;
					}
					if ((std::string)buff_rrr == "realname")
						newClient->SetUsername(newClient->GetNick());
					else
						newClient->SetUsername(buff_rrr);
					regfree(&regex);
					break ;
				}
			}
		}
    	// regfree(&regex);
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
	std::string message = RPL_WELCOME(newClient->GetNick(), newClient->GetUsername(), Command::getHostname()) + "\n"
		+ RPL_YOURHOST("RCI", "1.2.5") + "\n" + RPL_CREATED("today") + "\n" + RPL_MYINFO("RCI", "1.2.5", "i/t/k/o/l", "...") + "\n";
	send(fd_newClient, message.c_str(), message.size(), 0);
	std::cout << GREEN "Reply: RPL_WELCOME / RPL_YOURHOST / RPL_CREATED / RPL_MYINFO " << "[" << fd_newClient << "]" RESET << std::endl;
}

std::map<std::string, Channel*> Server::getChannelList()
{
	return (_channelList);
}

void Server::deleteChannel(std::string &channelName)
{
	_channelList.erase(channelName);
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
		std::cout << "Channel " + channelName + " successfully created" << std::endl;
		std::cout << "User " + oper->GetNick() + " successfully joined" << std::endl;
		broadcastMsg(":" + oper->GetNick() + "!" + oper->GetUsername() + "@" +
			Command::getHostname() + " JOIN " + channelName + " * :" + oper->GetRealname() + "\r\n", channelName, *oper, true);
		newChannel->broadcastUserList(*oper);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
