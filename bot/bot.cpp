/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:35:46 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/23 15:06:47 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

Bot::Bot(const std::string& server_ip, int port, const std::string& nickname, const std::string& username): server_ip(server_ip), port(port), nickname(nickname), username(username)
{
}

bool Bot::connectToServer()
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	{
		std::cerr << "Error: Unable to create socket" << std::endl;
		return (false);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());

	if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
	{
		std::cerr << "Error: Unable to connect to server" << std::endl;
		return (false);
	}
	return (true);
}

void Bot::sendRaw(const std::string& message)
{
	send(sockfd, message.c_str(), message.length(), 0);
}

void Bot::authenticate() 
{
	sendRaw("PASS popo\r\n");
	sleep(1);
	sendRaw("NICK " + nickname + "\r\n");
	sleep(1);
	sendRaw("USER " + username + " 0 * :" + username + "\r\n");
}

void Bot::joinChannel(const std::string& channel)
{
	sendRaw("JOIN " + channel + "\r\n");
}

void Bot::listen() 
{
	char buffer[1024];
	while (true) 
	{
		memset(buffer, 0, sizeof(buffer));
		int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
		if (bytes_received <= 0)
		{
			std::cerr << "Error: Connection closed or error occurred" << std::endl;
			break;
		}
		std::string message(buffer);
		std::cout << "Received: " << message << std::endl;

		if (trueMessage(message) == "PING\r")
		{
			std::cout << "coucou\n";
			std::string response = "PONG" + message.substr(4) + "\r\n";
			sendRaw(response);
		}
	}
}

std::string Bot::trueMessage(std::string message)
{
	std::string truncateMsg;

	std::size_t pos = message.rfind(":");
	
	if (pos != std::string::npos)
		truncateMsg = message.substr(pos + 1);
	// std::cout << "|" << truncateMsg << "|" << std::endl;
	return (truncateMsg);
}

Bot::~Bot()
{
	close(sockfd);
}
