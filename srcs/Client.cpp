/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:36:18 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/12 10:21:03 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Client.hpp"

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
		if (std::strncmp(buff_r, "USER ", 5) == 0)
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
			SetUsername(buff_rr);
			break ;
		}
	}	
}

