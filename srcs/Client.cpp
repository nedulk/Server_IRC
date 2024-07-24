/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:36:18 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/24 16:42:32 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): _bot(false)
{
	this->_Hostname = "";
	this->_Nickname = "";
	this->_Realname = "";
	this->_Username = "";
	this->_joinedChannels = 0;
}

Client::~Client()
{
}

bool Client::getBot()
{
	return (this->_bot);
}

void Client::setBot()
{
	if (this->_bot == false)
		this->_bot = true;
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

void Client::joinedChannel()
{
	_joinedChannels++;
}

void Client::leftChannel()
{
	_joinedChannels--;
}

int Client::getJoinedChannels()
{
	return (_joinedChannels);
}

void Client::sendErrorMsg(std::string msg)
{
	send(_fd, msg.c_str(), msg.size(), 0);
}
