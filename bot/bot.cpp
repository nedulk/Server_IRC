/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:35:46 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/28 16:50:52 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

bool Bot::_Signal = false;
int	 Bot::_sockfd = -1;

void Bot::SignalHandler(int signum)
{
	(void)signum;
	Bot::_Signal = true;
	
	std::string msg = "BOT QUIT\r\n";
	send(Bot::_sockfd, msg.c_str(), msg.size(), 0);
} 

Bot::Bot(const std::string& server_ip, int port, const std::string& nickname, const std::string& username, const std::string pass): _server_ip(server_ip), _port(port), _nickname(nickname), _username(username), _pass(pass)
{
	_sockfd = -1;
}

bool Bot::connectToServer()
{
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0) 
	{
		std::cerr << "Error: Unable to create socket" << std::endl;
		return (false);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port);
	server_addr.sin_addr.s_addr = inet_addr(_server_ip.c_str());
	

	if (connect(_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
	{
		std::cerr << "Error: Unable to connect to server" << std::endl;
		return (false);
	}
	return (true);
}

void Bot::sendRaw(const std::string& message)
{
	send(_sockfd, message.c_str(), message.length(), 0);
}

void Bot::authenticate() 
{
	sendRaw("PASS " + _pass + "\r\n");
	sendRaw("NICK " + _nickname + "\r\n");
	sendRaw("USER " + _username + " 0 * :" + _username + "\r\n");
	
}

void Bot::joinChannel(const std::string& channel)
{
	sleep(1);
	sendRaw("JOIN " + channel + "\r\n");
}
int Bot::listen() 
{
	char buffer[1024];
	while (Bot::_Signal == false) 
	{
		memset(buffer, 0, sizeof(buffer));
		int bytes_received = recv(_sockfd, buffer, sizeof(buffer) - 1, 0);
		if (bytes_received <= 0)
		{
			std::cerr << "Error: Connection closed or error occurred" << std::endl;
			return (1);
		}
		std::string message(buffer);
		std::cout << "Received: " << message;
		if (message.find("BOT QUIT") != std::string::npos)
			return (0);
		if (message.find("464 :Password incorrect") != std::string::npos || message.find("Erroneous nickname") != std::string::npos
			|| message.find("Nickname is already in use") != std::string::npos)
			return (1);

		std::string trucateMsg = trueMessage(message);
		if (trucateMsg == "PING")
		{
			std::string response = "PRIVMSG #PingBot :PONG\r\n";
			sendRaw(response);
		}
	}
	return (0);
}

bool isControlChar(char c)
{
    return std::iscntrl(static_cast<unsigned char>(c));
}

std::string Bot::trueMessage(std::string message)
{
	std::string truncateMsg;

	std::size_t pos = message.rfind(":");
	
	if (pos != std::string::npos)
	{
		truncateMsg = message.substr(pos + 1);
		truncateMsg.erase(std::remove_if(truncateMsg.begin(), truncateMsg.end(), isControlChar), truncateMsg.end());
	}
	return (truncateMsg);
}

Bot::~Bot()
{
	close(_sockfd);
}