/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:36:18 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/12 11:51:33 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	this->_Hostname = "";
	this->_Nickname = "";
	this->_Realname = "";
	this->_Username = "";
}

void Client::SetFd(int fd)
{
	this->_fd = fd;
}

int Client::GetFd()
{
	return (this->_fd);
}

void Client::SetIp(std::string ip)
{
	this->_IP = ip;
}

std::string Client::GetIp()
{
	return(this->_IP);
}

void Client::SetNick(std::string nickname)
{
	this->_Nickname = nickname;	
}

std::string Client::GetNick()
{
	return (this->_Nickname);
}

void Client::SetUsername(std::string username)
{
	this->_Username = username;	
}

std::string Client::GetUsername()
{
	return (this->_Username);
}

void Client::SetHostname(std::string hostname)
{
	this->_Hostname = hostname;	
}

std::string Client::GetHostname()
{
	return (this->_Hostname);
}

void Client::SetRealname(std::string realname)
{
	this->_Realname = realname;	
}

std::string Client::GetRealname()
{
	return (this->_Realname);
}

void Client::UserCheck(int fd_newClient)
{
	char buff_r[1024];
	for (int i = 0; i < 1024; i++)
		buff_r[i] = '\0';
	
	while (1)
	{
		recv(fd_newClient, buff_r, sizeof(buff_r) - 1, 0);
		regex_t regex;
		int ret;
		ret = regcomp(&regex, REGEXUSER, REG_EXTENDED);
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
				SetUsername(buff_rr);
				break ;
			}
		}
		regfree(&regex);
	}	
}

bool Client::isInvitedToChannel(std::string& channelName)
{
	for (std::vector<std::string>::iterator it = _channelInvites.begin(); it != _channelInvites.end(); it++)
	{
		if (*it == channelName)
			return (true);
	}
	return (false);
}

