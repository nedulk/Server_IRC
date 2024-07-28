/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:36:18 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/28 09:40:16 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

std::vector<std::string> Client::remain_line;

Client::Client()
{
	this->_Hostname = "";
	this->_Nickname = "";
	this->_Realname = "";
	this->_Username = "";
	this->pass_check = 0;
	this->nick_check = 0;
	this->user_check = 0;
}

Client::~Client()
{
}

void Client::splitAndKeepLastTwo(const std::string& str) 
{
	char* cstr = new char[str.length() + 1];
	std::strcpy(cstr, str.c_str());

	char* token = std::strtok(cstr, "\n\r");
	while (token != NULL) 
	{
		std::string tokenStr = token;
		tokenStr.erase(std::remove(tokenStr.begin(), tokenStr.end(), '\n'), tokenStr.end());
		tokenStr.erase(std::remove(tokenStr.begin(), tokenStr.end(), '\r'), tokenStr.end());
		remain_line.push_back(tokenStr);
		token = std::strtok(NULL, "\n\r");
	}
	delete[] cstr;
}

std::string Client::compare_remain_line(const std::string& str)
{
	for (std::vector<std::string>::iterator it = remain_line.begin(); it != remain_line.end(); ++it)
	{
		if (std::strncmp(str.c_str(), it->c_str(), 5) == 0)
			return (*it);
	}
	return ("");
}

int Client::getUserCheck()
{
	return (user_check);	
}

int Client::getPassCheck()
{
	return (pass_check);
}

int Client::getNickCheck()
{
	return (nick_check);
}

void Client::setUserCheck(int i)
{
	user_check = i;	
}

void Client::setPassCheck(int i)
{
	pass_check = i;
}

void Client::setNickCheck(int i)
{
	nick_check = i;
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

void Client::joinedChannel(Channel *channel)
{
	_joinedChannels.push_back(channel);
}

void Client::leftChannel(Channel *channel)
{
	for (std::vector<Channel*>::iterator it = _joinedChannels.begin(); it != _joinedChannels.end(); ++it)
	{
		if (*it == channel)
		{
			_joinedChannels.erase(it);
			break ;
		}
	}
}

std::vector<Channel*> Client::getJoinedChannels()
{
	return (_joinedChannels);
}

void Client::sendErrorMsg(std::string msg)
{
	send(_fd, msg.c_str(), msg.size(), 0);
}

std::string Client::getClientBuff() {
	return (_buffer);
}

void Client::setClientBuff(std::string msg)
{
	_buffer = msg;
}
